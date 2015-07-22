#ifndef IACCESSREGWRITER_H
#define IACCESSREGWRITER_H

#include <Arduino.h>

#include "AccessReg.h"

/*
* Classe respons�vel por gravar e deletar um registro com acesso permitido.
*/
class IAccessRegWriter {

public:
	/*
	* Cria uma inst�ncia de IAccessRegWriter.
	*/
	IAccessRegWriter() { }
	
	/*
	* Armazena o registro {@code value} fornecido.
	* 
	* @param value O registro com acesso permitido a ser armazenado na controladora.
	* @return Retorna 1 quando o registro for salvo com sucesso ou, caso contr�rio, -1.
	*/
	virtual int				Write( const AccessReg value ) = 0;
	/*
	* Busca o registro de c�digo {@code value} e o deleta.
	* 
	* @param value O c�digo do registro a ser deletado.
	* @return Retorna 1 quando o registro for deletado com sucesso ou, caso contr�rio, -1.
	*/
	virtual int				Delete( const byte code[4] ) = 0;

	/*
	* Busca o registro de c�digo {@code code} e hidrata o AccessReg dado em {@code target} .
	* 
	* @param value O c�digo do registro a ser buscado.
	* @param target O objeto a ser hidratado quando o registro for encontrado.
	* @return Retorna 1 quando o registro � encontrado ou, caso contr�rio, -1.
	*/
	virtual int				Get( const byte code[4], AccessReg &target ) = 0;

	/*
	* Deleta todos os registros associados a esta inst�ncia.
	*/ 
	virtual bool			Clear() = 0;

	virtual					~IAccessRegWriter()	{ }
};

#endif /* IACCESSREGWRITER_H */