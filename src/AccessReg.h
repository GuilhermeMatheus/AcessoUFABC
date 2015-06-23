#ifndef ACCESSREG_H
#define ACCESSREG_H

#include <Arduino.h>

#define PACK_REG_SIZE 9

/**
* Representa um registro Mestre ou de um cart�o com acesso permitido.
*/
struct AccessReg {
	/**
	* C�digo do cart�o de acesso.
	*/
	byte						mifareID[4];
	/**
	* Marca se o registro � Master ou um registro comum.
	* Quando seu valor � 1, esta entidade representa um Master, caso contr�rio, um registro de acesso comum.
	*/
	byte						isMaster			: 1;
	/**
	* Indica os dias da semana em que o acesso � permitido.
	* Seus valores prim�rios s�o:
	*	[0000001] : Domingo
	*	[0000010] : Segunda feira
	*	[0000100] : Ter�a feira
	*	[0001000] : Quarta feira
	*	[0010000] : Quinta feira
	*	[0100000] : Sexta feira
	*	[1000000] : S�bado
	* 
	* Composi��es de v�rios dias por semana s�o poss�veis a partir da soma dos valores prim�rios.
	*/
	byte						allowedDaysOfWeek	: 7;
	/**
	* Hora de in�cio do per�odo em que o acesso � permitido.
	*/
	byte						hourStart			: 5;
	/**
	* Hora de fim do per�odo em que o acesso � permitido.
	*/
	byte						hourEnd				: 5;
	/**
	* Dia do fim da validade deste registro.
	*/
	byte						untilDay			: 5;
	/**
	* M�s do fim da validade deste registro.
	*/
	byte						untilMon			: 4;
	/**
	* Ano do fim da validade deste registro.
	*/
	uint16_t					untilYear			: 13;
};

/**
* Classe utilit�ria na manipula��o de {@code AccessReg}.
*/
class AccessRegUtils {

public:
	/**
	* Realiza a compacta��o de {@code reg} e armazena seu valor em {@code buffer}.
	* 
	* @param reg O registro a ser compactado.
	* @param buffer O buffer de destino dos valores compactados. Deve possui NO M�NIMO 9 bytes.
	*/
	static void					AccessReg_pack( const AccessReg &reg, byte buffer[] );
	/**
	* Realiza a descompacta��o de {@code buffer} e armazena seu valor em {@code reg}.
	* 
	* @param reg O registro a ser preenchido com os valores de {@code buffer}.
	* @param buffer O buffer que armazena os valores do registro.
	*/
	static void					AccessReg_unpack( const byte buffer[], AccessReg &reg );
};


#endif /* ACCESSREG_H */