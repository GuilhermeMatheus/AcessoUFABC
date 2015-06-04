#ifndef IIDPROVIDER_H

#define IIDPROVIDER_H
#define EMPTY_BYTE		0xFF

#include "Arduino.h"
#include "System.h"

class IIDProvider {
public:

	/**
	* Escreve os a sequ�ncida de bytes retornada pelo ID.
	* @param value Endere�o a ser gravado o ID
	* @return Retorna 1 se gravou o byte, caso contr�rio,
	*		  retorna -1.
	*/
	virtual int				GetNextID( byte &value ) = 0;
	virtual int				WaitForNextID( byte &value, long timeLimit ) {
		System::LED_GREEN_ON();

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

		System::LED_GREEN_OFF();
		return result;
	}

	virtual					~IIDProvider() { }
};

#endif /* IIDPROVIDER_H */