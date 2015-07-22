#ifndef AT24C256REGWRITER_H
#define AT24C256REGWRITER_H

#include "IAccessRegWriter.h"

class AT24C256RegWriter :
	public IAccessRegWriter
{
	
private:
	int						findSlot( const byte code[4] );

public:

	AT24C256RegWriter();

	int						Write( const AccessReg value );
	int						Delete( const byte code[4] );
	int						Get( const byte code[4], AccessReg &target );
	bool					Clear();

	~AT24C256RegWriter();
};

#endif /* AT24C256REGWRITER_H */