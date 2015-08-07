#ifndef IIDPROVIDER_H

#define IIDPROVIDER_H
#define EMPTY_BYTE		0xFF

#include "Arduino.h"
#include "System.h"

/**
* Classe responsável por interfacear dispositivos de leitura de ID.
*/ 
class IIDProvider {
public:

	/**
	* Escreve os a sequêncida de bytes retornada pelo ID.
	* 
	* @param value Endereço a ser gravado o ID.
	* @return Retorna 1 se gravou o byte, caso contrário, retorna -1.
	*/
	virtual int				GetNextID( byte &value ) = 0;

	/**
	* Pausa a aplicação e aguarda um novo ID disponível.
	* 
	* @param value Endereço a ser gravado o ID.
	* @param timeLimit Tempo limite a aguardar o novo ID.
	* @return Retorna 1 se um novo ID estiver disponível no tempo determinado ou, caso contrário, retorna -1.
	*/
	virtual int				WaitForNextID( byte &value, long timeLimit ) {
		int result = 1;
		long elapsed = 0;

		while ( GetNextID(value) < 0 ) {
			delay(300);
			elapsed += 300;
			if ( elapsed >= timeLimit ) {
				result = -1;
				break;
			}
		}

		return result;
	}

	virtual					~IIDProvider() { }
};

#endif /* IIDPROVIDER_H */