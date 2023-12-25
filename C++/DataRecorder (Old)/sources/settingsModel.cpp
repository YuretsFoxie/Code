#include "settingsModel.h"
#include "file.h"

#include <math.h>

void SettingsModel::save()
{
	File("model_settings.txt").save(data);
}

void SettingsModel::load()
{
	data = File("model_settings.txt").load();
}

void SettingsModel::increment()
{
	if (state == TIME && data[TIME] < -1)		data[TIME] += 1;
	if (state == SPEED && data[SPEED] <= 9)	data[SPEED] += 1;
}

void SettingsModel::decrement()
{
	if (state == TIME && data[TIME] > -10)		data[TIME] -= 1;
	if (state == SPEED && data[SPEED] >= -9)	data[SPEED] -= 1;
}

void SettingsModel::setModeTime()
{
	state = TIME;
}

void SettingsModel::setModeSpeed()
{
	state = SPEED;
}

int SettingsModel::getTime()
{
	if (data.empty()) return 1;
	
	return pow(2, -data[TIME]);
}

int SettingsModel::getSpeed()
{
	if (data.empty()) return 0;
	if (data[SPEED] == 0) return 0;
	
	return data[SPEED] < 0 ? -pow(2, -data[SPEED]) : pow(2, data[SPEED]);
}
