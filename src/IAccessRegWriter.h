#ifndef IACCESSREGWRITER_H
#define IACCESSREGWRITER_H

#include <Arduino.h>

#include "AccessReg.h"

/*
* Classe responsável por gravar e deletar um registro com acesso permitido.
*/
class IAccessRegWriter {

public:
	/*
	* Cria uma instância de IAccessRegWriter.
	*/
	IAccessRegWriter() { }
	
	/*
	* Armazena o registro {@code value} fornecido.
	* 
	* @param value O registro com acesso permitido a ser armazenado na controladora.
	* @return Retorna 1 quando o registro for salvo com sucesso ou, caso contrário, -1.
	*/
	virtual int				Write( AccessReg value ) = 0;
	/*
	* Busca o registro de código {@code value} e o deleta.
	* 
	* @param value O código do registro a ser deletado.
	* @return Retorna 1 quando o registro for deletado com sucesso ou, caso contrário, -1.
	*/
	virtual int				Delete( byte code[4] ) = 0;

	virtual					~IAccessRegWriter()	{ }
};

#endif /* IACCESSREGWRITER_H */