#ifndef IMIFAREIDPROVIDER_H
#define IMIFAREIDPROVIDER_H

#include "Arduino.h"

class IMifareIDProvider
{
public:
	virtual bool HasNext() = 0;

	virtual byte GetID() = 0;

	virtual ~IMifareIDProvider() = 0;
};

#endif /* IMIFAREIDPROVIDER_H */