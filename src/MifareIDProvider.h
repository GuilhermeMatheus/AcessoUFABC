#ifndef MIFAREIDPROVIDER_H
#define MIFAREIDPROVIDER_H

#include "IIDProvider.h"

#include <MFRC522.h>

class MifareIDProvider :
	public IIDProvider {
protected:
	MFRC522*				reader;

public:
	MifareIDProvider( MFRC522* reader );

	int						GetNextID( byte &value );
};

#endif /* MIFAREIDPROVIDER_H */