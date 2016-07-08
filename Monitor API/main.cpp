#include <windows.h>
#include <HighLevelMonitorConfigurationAPI.h>
#include <PhysicalMonitorEnumerationAPI.h>
#include <iostream>
#include <vector>

//TEMPORARY TESTING, WILL BE OOP LATER

int main(int, char**);
BOOL CALLBACK MonitorEnumCallback(
	_In_ HMONITOR hMonitor,
	_In_ HDC      hdcMonitor,
	_In_ LPRECT   lprcMonitor,
	_In_ LPARAM   dwData
);

//Array of monitor handles
std::vector<HMONITOR> monitors;
std::vector<LPPHYSICAL_MONITOR> pMonitors;

BOOL CALLBACK monitorEnumCallback(
	_In_ HMONITOR hMonitor,
	_In_ HDC      hdcMonitor,
	_In_ LPRECT   lprcMonitor,
	_In_ LPARAM   dwData
)
{
	monitors.push_back(hMonitor);
	return TRUE;
}

int main(int argc, char **argv)
{
	//Find any connected monitors
	if (EnumDisplayMonitors(NULL, NULL, monitorEnumCallback, NULL) == 0)
	{
		//TODO: Proper Error checking
		std::cout << "Could not find Monitor Handles" << std::endl;
	}

	//Display monitor details
	for (UINT16 currentMonitor = 0; currentMonitor < monitors.size(); ++currentMonitor)
	{
		HMONITOR monitor = monitors[currentMonitor];
		
		MONITORINFOEX monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFOEX); //Need to set this to allow next function to identify Structure type

		if (GetMonitorInfo(monitor, &monitorInfo) == 0)
		{
			std::cout << "Monitor " << currentMonitor << " is not providing info" << std::endl;
		}
		else 
		{
			std::cout << "Monitor " << currentMonitor << ":" << std::endl;
			if (monitorInfo.dwFlags == MONITORINFOF_PRIMARY)
			{
				std::cout << "Is Primary Monitor" << std::endl;
			}
			std::cout << "Name: " << monitorInfo.szDevice << std::endl;
		}

		DWORD monCount = 0;
		GetNumberOfPhysicalMonitorsFromHMONITOR(monitor, &monCount);

		LPPHYSICAL_MONITOR pMonitor = (LPPHYSICAL_MONITOR)malloc(monCount * sizeof(PHYSICAL_MONITOR));

		GetPhysicalMonitorsFromHMONITOR(monitor, monCount, pMonitor);
		


		DWORD minBright = 0;
		DWORD maxBright = 0;
		DWORD currentBright = 0;
		if (GetMonitorBrightness(pMonitor[0].hPhysicalMonitor, &minBright, &currentBright, &maxBright) == FALSE)
		{
			std::cout << "error: " << GetLastError() << std::endl;
		}

		std::cout << std::endl << "Minimum Brightness: " << minBright << std::endl;
		std::cout << "Current Brightness: " << currentBright << std::endl;
		std::cout << "Maximum Brightness: " << maxBright << std::endl;
		
		SetMonitorBrightness(pMonitor[0].hPhysicalMonitor, 100);
		
		//pMonitors.push_back(pMonitor);
	}

	

	return 0;
}