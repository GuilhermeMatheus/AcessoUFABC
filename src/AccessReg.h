#ifndef ACCESSREG_H
#define ACCESSREG_H

#include <Arduino.h>

#define PACK_REG_SIZE 9

/**
* Representa um registro Mestre ou de um cartão com acesso permitido.
*/
struct AccessReg {
	/**
	* Código do cartão de acesso.
	*/
	byte						mifareID[4];
	/**
	* Marca se o registro é Master ou um registro comum.
	* Quando seu valor é 1, esta entidade representa um Master, caso contrário, um registro de acesso comum.
	*/
	byte						isMaster			: 1;
	/**
	* Indica os dias da semana em que o acesso é permitido.
	* Seus valores primários são:
	*	[0000001] : Domingo
	*	[0000010] : Segunda feira
	*	[0000100] : Terça feira
	*	[0001000] : Quarta feira
	*	[0010000] : Quinta feira
	*	[0100000] : Sexta feira
	*	[1000000] : Sábado
	* 
	* Composições de vários dias por semana são possíveis a partir da soma dos valores primários.
	*/
	byte						allowedDaysOfWeek	: 7;
	/**
	* Hora de início do período em que o acesso é permitido.
	*/
	byte						hourStart			: 5;
	/**
	* Hora de fim do período em que o acesso é permitido.
	*/
	byte						hourEnd				: 5;
	/**
	* Dia do fim da validade deste registro.
	*/
	byte						untilDay			: 5;
	/**
	* Mês do fim da validade deste registro.
	*/
	byte						untilMon			: 4;
	/**
	* Ano do fim da validade deste registro.
	*/
	uint16_t					untilYear			: 13;
};

/**
* Classe utilitária na manipulação de {@code AccessReg}.
*/
class AccessRegUtils {

public:
	/**
	* Realiza a compactação de {@code reg} e armazena seu valor em {@code buffer}.
	* 
	* @param reg O registro a ser compactado.
	* @param buffer O buffer de destino dos valores compactados. Deve possui NO MÍNIMO 9 bytes.
	*/
	static void					AccessReg_pack( const AccessReg &reg, byte buffer[] );
	/**
	* Realiza a descompactação de {@code buffer} e armazena seu valor em {@code reg}.
	* 
	* @param reg O registro a ser preenchido com os valores de {@code buffer}.
	* @param buffer O buffer que armazena os valores do registro.
	*/
	static void					AccessReg_unpack( const byte buffer[], AccessReg &reg );
};


#endif /* ACCESSREG_H */