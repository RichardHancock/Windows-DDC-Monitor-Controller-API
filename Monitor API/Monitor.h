#pragma once

#include <windows.h>
#include <HighLevelMonitorConfigurationAPI.h>
#include <PhysicalMonitorEnumerationAPI.h>
#include <string>

class Monitor {

public:

	Monitor(HMONITOR nonPhysicalMonitor);

	~Monitor();

	bool isValid();

	

	enum ColourTemps {
		Temp4000k,
		Temp5000k,
		Temp6500k,
		Temp7500k,
		Temp8200k,
		Temp9300k,
		Temp10000k,
		Temp11500k,
		ENUM_ITEM_COUNT = 8
	};

protected:

	PHYSICAL_MONITOR monitorPointer;

	std::string name;

	std::string getName();

	bool valid;

	bool primaryMonitor;

	bool initialiseMonitor(HMONITOR nonPhysicalMonitor);


	//Capabilities
	bool getCapabilities();

	unsigned long capabilities;


	bool supportsBrightness();

	bool supportsColourTemperature();

	bool supportsContrast();

	//CRT Only, just keeping for completeness
	bool supportsDegauss();

	bool supportsDisplayAreaPosition();

	bool supportsDisplayAreaSize();

	bool supportsTechnologyType();

	bool supportsRGBDrive();

	bool supportsRGBGain();

	bool supportsRestoreColourDefaults();

	bool supportsRestoreDefaults();

	bool supportsRestoreDefaultEX();

	//Colour Capabilities
	unsigned long colourCapabilities;

	bool supportsSpecificColourTemperature(ColourTemps colourTemperature);

	unsigned long colourFlags[ColourTemps::ENUM_ITEM_COUNT];
};