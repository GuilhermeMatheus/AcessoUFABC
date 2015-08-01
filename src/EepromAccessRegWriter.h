#ifndef EEPROMACCESSREGWRITER_H
#define EEPROMACCESSREGWRITER_H

#include <EEPROM.h>

#include "System.h"
#include "IAccessRegWriter.h"

/*
* Classe respons�vel por gravar e deletar um registro com acesso permitido na mem�ria EEPROM do microprocessador.
*/
class EepromAccessRegWriter :
	public IAccessRegWriter {

private:
	IAccessRegWriter		*fallback;
	int						findSlot( const byte code[4] );

public:
	EepromAccessRegWriter( IAccessRegWriter *fallback );

	int						Write( const AccessReg value );
	int						Delete( const byte code[4] );
	int						Get( const byte code[4], AccessReg &target );
	bool					Clear();
	
	~EepromAccessRegWriter();
};

#endif /* EEPROMACCESSREGWRITER_H */