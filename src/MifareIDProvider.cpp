#include "MifareIDProvider.h"

MifareIDProvider::MifareIDProvider(MFRC522* reader)
{
	this->reader = reader;
}

int MifareIDProvider::GetNextID(byte &value)
{
	bool foundCard = reader->PICC_IsNewCardPresent() && reader->PICC_ReadCardSerial();

	if (!foundCard)
		return -1;

	byte size = reader->uid.size;
	
	for (byte i = 0; i < size; i++)
		(&value)[i] = EMPTY_BYTE;

	for (byte i = 0; i < size; i++)
		(&value)[i] = reader->uid.uidByte[i];

	return 1;
}
