#include "System.h"
#include "EepromAccessRegWriter.h"
#include <EEPROM.h>


EepromAccessRegWriter::EepromAccessRegWriter()
{
}

int EepromAccessRegWriter::Write(AccessReg value)
{
	int ee = findEmptySlot();
	
	if (ee < 0)
		return ee;

	const byte* p = (const byte*)(const void*)&value;
	for (int i = 0; i < sizeof(value); i++)
		EEPROM.write(ee++, *p++);

	return 1;
}

int EepromAccessRegWriter::findEmptySlot()
{
	for (int i = OFFSET_ACS_Password; i < 1024; i += sizeof(AccessReg))
	{
		boolean found = true;
		for (int j = i; j < i + sizeof(AccessReg); j++)
			if (EEPROM.read(i) != 0xFF)
			{
				found = false;
				break;
			}

		if (found)
			return i;
	}

	return -1;
}

EepromAccessRegWriter::~EepromAccessRegWriter()
{
}
