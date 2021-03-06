#include <windows.h>
#include <HighLevelMonitorConfigurationAPI.h>
#include <PhysicalMonitorEnumerationAPI.h>
#include <iostream>
#include <vector>

#include "Monitor.h"
#include "LaptopBrightness.h"

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
	bool argumentProvided = false;
	unsigned long newBrightness = 0;

	if (argc > 1)
	{
		newBrightness = atoi(argv[1]);
		if (newBrightness < 0 || newBrightness > 100)
		{
			return -1;
		}
		
		argumentProvided = true;
	}

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
		
			
			#ifndef NDEBUG
				newMonitor->printCapabilities();
			#endif
			
		}
	}

	//LaptopBrightness* laptop = new LaptopBrightness();
	//delete laptop;
	
	for (Monitor* monitor : monitors)
	{
		if (argumentProvided)
			monitor->setBrightness(newBrightness);
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