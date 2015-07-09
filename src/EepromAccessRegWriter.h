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
	int						findSlot( const byte code[4] );

public:
	EepromAccessRegWriter();

	int						Write( const AccessReg value );
	int						Delete( const byte code[4] );
	int						Get( const byte code[4], AccessReg &target );
	
	~EepromAccessRegWriter();
};

#endif /* EEPROMACCESSREGWRITER_H */