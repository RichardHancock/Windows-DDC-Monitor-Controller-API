#include "Monitor.h"

#include <iostream>
#include <stdlib.h>

Monitor::Monitor(HMONITOR nonPhysicalMonitor)
{
	primaryMonitor = false;
	valid = false;
	name = "Undefined";
	capabilities = 0;
	colourCapabilities = 0;

	//Populate colourFlags array
	colourFlags[Temp4000k] = MC_SUPPORTED_COLOR_TEMPERATURE_4000K;
	colourFlags[Temp5000k] = MC_SUPPORTED_COLOR_TEMPERATURE_5000K;
	colourFlags[Temp6500k] = MC_SUPPORTED_COLOR_TEMPERATURE_6500K;
	colourFlags[Temp7500k] = MC_SUPPORTED_COLOR_TEMPERATURE_7500K;
	colourFlags[Temp8200k] = MC_SUPPORTED_COLOR_TEMPERATURE_8200K;
	colourFlags[Temp9300k] = MC_SUPPORTED_COLOR_TEMPERATURE_9300K;
	colourFlags[Temp10000k] = MC_SUPPORTED_COLOR_TEMPERATURE_10000K;
	colourFlags[Temp11500k] = MC_SUPPORTED_COLOR_TEMPERATURE_11500K;


	initialiseMonitor(nonPhysicalMonitor);


	//Fetch Monitor name
	//Would love some advice on how to deal with WCHAR properly
	char* nameRaw = new char[128];
	size_t length;

	///@TODO Find better method
	if (wcstombs_s(&length, nameRaw, 128, monitorPointer.szPhysicalMonitorDescription, 128) != 0)
	{
		std::cout << "Error in converting wide to char array for monitor name." << std::endl;
	}

	name = nameRaw;

	delete nameRaw;

	getCapabilities();
}

Monitor::~Monitor()
{
	//Release Handles
	DestroyPhysicalMonitor(monitorPointer.hPhysicalMonitor);
}


std::string Monitor::getName()
{
	return name;
}

void Monitor::printCapabilities()
{
	std::cout << "Capabilities:" << std::endl;
	std::cout << "	Brightness: " << supportsBrightness() << std::endl;
	std::cout << "	Contrast: " << supportsContrast() << std::endl;
	std::cout << "	Degauss: " << supportsDegauss() << std::endl;
	std::cout << "	Display Area Position: " << supportsDisplayAreaPosition() << std::endl;
	std::cout << "	Display Area Size: " << supportsDisplayAreaSize() << std::endl;
	std::cout << "	Technology Type: " << supportsTechnologyType() << std::endl;
	std::cout << "	RGB Drive: " << supportsRGBDrive() << std::endl;
	std::cout << "	RGB Gain: " << supportsRGBGain() << std::endl;
	std::cout << "	Restore Colour Defaults: " << supportsRestoreColourDefaults() << std::endl;
	std::cout << "	Restore Defaults: " << supportsRestoreDefaults() << std::endl;
	std::cout << "	Restore Defaults Extended: " << supportsRestoreDefaultEX() << std::endl;

	std::cout << "	Colour Temperature: " << supportsColourTemperature() << std::endl;
	if (supportsColourTemperature())
	{
		std::cout << "		4000k: " << supportsSpecificColourTemperature(Temp4000k) << std::endl;
		std::cout << "		5000k: " << supportsSpecificColourTemperature(Temp5000k) << std::endl;
		std::cout << "		6500k: " << supportsSpecificColourTemperature(Temp6500k) << std::endl;
		std::cout << "		7500k: " << supportsSpecificColourTemperature(Temp7500k) << std::endl;
		std::cout << "		8200k: " << supportsSpecificColourTemperature(Temp8200k) << std::endl;
		std::cout << "		9300k: " << supportsSpecificColourTemperature(Temp9300k) << std::endl;
		std::cout << "		10000k: " << supportsSpecificColourTemperature(Temp10000k) << std::endl;
		std::cout << "		11500k: " << supportsSpecificColourTemperature(Temp11500k) << std::endl;
	}
}

bool Monitor::initialiseMonitor(HMONITOR nonPhysicalMonitor)
{
	if (nonPhysicalMonitor == NULL)
	{
		std::cout << "No monitor handle passed to Monitor Class";
		valid = false;
		return false;
	}

	MONITORINFOEX monitorInfo;
	monitorInfo.cbSize = sizeof(MONITORINFOEX); //Need to set this to allow next function to identify Structure type

	if (GetMonitorInfo(nonPhysicalMonitor, &monitorInfo) == 0)
	{
		std::cout << "Monitor is not providing info" << std::endl;
	}
	else
	{
		if (monitorInfo.dwFlags == MONITORINFOF_PRIMARY)
		{
			std::cout << "Is Primary Monitor" << std::endl;
			primaryMonitor = true;
		}
		std::cout << "Sys Name: " << monitorInfo.szDevice << std::endl;
	}

	DWORD monCount = 0;
	GetNumberOfPhysicalMonitorsFromHMONITOR(nonPhysicalMonitor, &monCount);

	LPPHYSICAL_MONITOR pMonitor = (LPPHYSICAL_MONITOR)malloc(monCount * sizeof(PHYSICAL_MONITOR));

	GetPhysicalMonitorsFromHMONITOR(nonPhysicalMonitor, monCount, pMonitor);

	monitorPointer = pMonitor[0];

	valid = true;
	return true;
}

bool Monitor::isValid()
{
	return valid;
}

bool Monitor::getCapabilities()
{
	if (!GetMonitorCapabilities(monitorPointer.hPhysicalMonitor, &capabilities, &colourCapabilities))
	{
		std::cout << "Monitor does not seem to support DDC/CI and is thus unusable with this program." << std::endl;
		std::cout << "Extended error info: " << GetLastError() << std::endl;
		return false;
	}

	return true;
}

bool Monitor::supportsBrightness()
{
	return (capabilities & MC_CAPS_BRIGHTNESS) != 0;
}

bool Monitor::supportsColourTemperature()
{
	return (capabilities & MC_CAPS_COLOR_TEMPERATURE) != 0;
}

bool Monitor::supportsContrast()
{
	return (capabilities & MC_CAPS_CONTRAST) != 0;
}

bool Monitor::supportsDegauss()
{
	return (capabilities & MC_CAPS_DEGAUSS) != 0;
}

bool Monitor::supportsDisplayAreaPosition()
{
	return (capabilities & MC_CAPS_DISPLAY_AREA_POSITION) != 0;
}

bool Monitor::supportsDisplayAreaSize()
{
	return (capabilities & MC_CAPS_DISPLAY_AREA_SIZE) != 0;
}

bool Monitor::supportsTechnologyType()
{
	return (capabilities & MC_CAPS_MONITOR_TECHNOLOGY_TYPE) != 0;
}

bool Monitor::supportsRGBDrive()
{
	return (capabilities & MC_CAPS_RED_GREEN_BLUE_DRIVE) != 0;
}

bool Monitor::supportsRGBGain()
{
	return (capabilities & MC_CAPS_RED_GREEN_BLUE_GAIN) != 0;
}

bool Monitor::supportsRestoreColourDefaults()
{
	return (capabilities & MC_CAPS_RESTORE_FACTORY_COLOR_DEFAULTS) != 0;
}

bool Monitor::supportsRestoreDefaults()
{
	return (capabilities & MC_CAPS_RESTORE_FACTORY_DEFAULTS) != 0;
}

bool Monitor::supportsRestoreDefaultEX()
{
	return (capabilities & MC_RESTORE_FACTORY_DEFAULTS_ENABLES_MONITOR_SETTINGS) != 0;
}

bool Monitor::supportsSpecificColourTemperature(ColourTemps colourTemperature)
{
	return (colourCapabilities & colourFlags[colourTemperature]) != 0;	
}
