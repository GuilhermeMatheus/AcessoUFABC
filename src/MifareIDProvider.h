#ifndef MIFAREIDPROVIDER_H
#define MIFAREIDPROVIDER_H

#include "IIDProvider.h"

#include <MFRC522.h>

/**
* Classe responsável por fornecer IDs lidos através do protocolo Mifare
*/ 
class MifareIDProvider :
	public IIDProvider {
protected:
	MFRC522*				reader;

public:
	/**
	* Cria uma intância de MifareIDProvider
	* 
	* @param reader Leitor a ser consultado
	*/ 
	MifareIDProvider( MFRC522* reader );

	int						GetNextID( byte &value );
};

#endif /* MIFAREIDPROVIDER_H */