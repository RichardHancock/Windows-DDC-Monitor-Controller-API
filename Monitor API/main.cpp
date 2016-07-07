#include <windows.h>
#include <HighLevelMonitorConfigurationAPI.h>
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

		
	}

	
	return 0;
}