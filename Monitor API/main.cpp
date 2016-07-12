#include <windows.h>
#include <HighLevelMonitorConfigurationAPI.h>
#include <PhysicalMonitorEnumerationAPI.h>
#include <iostream>
#include <vector>

#include "Monitor.h"

//TEMPORARY TESTING, WILL BE OOP LATER

int main(int, char**);
BOOL CALLBACK MonitorEnumCallback(
	_In_ HMONITOR hMonitor,
	_In_ HDC      hdcMonitor,
	_In_ LPRECT   lprcMonitor,
	_In_ LPARAM   dwData
);

//Array of monitor handles
std::vector<HMONITOR> monitorsRaw;

BOOL CALLBACK monitorEnumCallback(
	_In_ HMONITOR hMonitor,
	_In_ HDC      hdcMonitor,
	_In_ LPRECT   lprcMonitor,
	_In_ LPARAM   dwData
)
{
	//HMONITORS don't need to be freed or memory managed
	monitorsRaw.push_back(hMonitor);
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

	std::vector<Monitor*> monitors;

	//Display monitor details
	for (UINT16 currentMonitor = 0; currentMonitor < monitorsRaw.size(); ++currentMonitor)
	{
		Monitor* newMonitor = new Monitor(monitorsRaw[currentMonitor]);
		if (!newMonitor->isValid())
		{
			//handle Error
		}
		else
		{
			monitors.push_back(newMonitor);
			std::cout << newMonitor->getName() << " Connected" << std::endl;
			newMonitor->printCapabilities();
		}
	}

	
	//Clean Up
	for (Monitor* monitor : monitors)
	{
		delete monitor;
	}
	monitors.clear();
	monitorsRaw.clear();

	return 0;
}

/* Brightness Function (Tested Working needs integration to Class)
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

SetMonitorBrightness(pMonitor[0].hPhysicalMonitor, 0);
*/