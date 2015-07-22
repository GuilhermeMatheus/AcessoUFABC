#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>
#include <RTClib.h>
#include <EEPROM.h>

#include "utils.h"
#include "AccessReg.h"
#include "IAccessRegWriter.h"

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define MAX_EEPROM_SIZE		  4096
#else
#define MAX_EEPROM_SIZE		  1024
#endif

#define OFFSET_ACS_Password		  0
#define OFFSET_NW_IsDHCP		  4
#define OFFSET_NW_IpAddress		  5
#define OFFSET_NW_Mask			  9 
#define OFFSET_NW_Gateway		 13
#define OFFSET_SRV_IpAddress	 17
#define OFFSET_SRV_Port			 21
#define OFFSET_ACT_Type			 23
#define OFFSET_ACT_Time			 24
#define OFFSET_DT_NTPIpAddress	 26
#define OFFSET_DT_UseNTP		 30
#define OFFSET_SRV_Computer		 31
#define OFFSET_NW_TerminalNumber 32
#define OFFSET_SETUP			 33
#define OFFSET_ACS_Regs			 34

#define LED_RED_PIN				 8
#define GATE_PIN				 6
#define LED_GREEN_PIN			 7
#define BUZZER_PIN				 5

#pragma warning( push )
#pragma warning( disable : 4068) //warning C4068: unknown pragma

class System {
private:
	static IAccessRegWriter		*accessWriter;
	System();
	~System();

public:
	static void					begin( IAccessRegWriter *accessWriter );


/*
===============================================================================
Visual commands.
===============================================================================
*/
#pragma region Visual
	/**
	* Desliga todos os leds.
	*/
	static void				LED_ALL_OFF();

	/**
	* Liga o led vermelho.
	*/
	static void				LED_RED_ON();
	
	/**
	* Desliga o led vermelho.
	*/
	static void				LED_RED_OFF();
	
	/**
	* Liga o led vermelho, pausa a execu��o do programa e ent�o desliga o led.
	* 
	* @param duration O tempo, em millisegundos, para pausar a aplica��o.
	*/
	static void				LED_RED_BLINK( unsigned long duration = 100 );
	
	/**
	* Liga o led verde.
	*/
	static void				LED_GREEN_ON();
	
	/**
	* Desliga o led verde.
	*/
	static void				LED_GREEN_OFF();
	
	/**
	* Liga o led verde, pausa a execu��o do programa e ent�o desliga o led.
	* 
	* @param duration O tempo, em millisegundos, para pausar a aplica��o.
	*/
	static void				LED_GREEN_BLINK( unsigned long duration = 100 );
	
	/**
	* Liga o buzzer, pausa a execu��o do programa e ent�o desliga o buzzer.
	* 
	* @param duration O tempo, em millisegundos, para pausar a aplica��o.
	*/
	static void				BEEP( unsigned long duration = 100 );

	/**
	* Liga o led vermelho, faz 3 BEEPs e pausa a aplica��o
	*
	* @param duration O tempo, em millisegundos, para pausar a aplica��o.
	*/
	static void				NOTIFY_ERROR( unsigned long duration = 100 );

/*

*/


#pragma endregion


/*
===============================================================================
Access settings.
===============================================================================
*/
#pragma region Access
	/**
	* Remove o acesso do registro com o c�digo {@code code} da mem�ria interna da controladora.
	* 
	* @param code O ID de 4 bytes do registro.
	* @return True quando o registro for removido com sucesso ou, caso contr�rio, False.
	*/
	static bool				ACS_RevokeCard( const byte code[] );
	
	/**
	* Grava o registro na mem�ria interna da controladora.
	* 
	* @param value O registro a ser salvo na mem�ria interna da controladora.
	* @return True quando o registro for inserido com sucesso ou, caso contr�rio, False.
	*/
	static bool				ACS_AddAccessReg( const AccessReg &value );
		
	/**
	* Adiciona o c�digo {@code card} na lista de c�digos administradores da controladora.
	* 
	* @param card O ID de 4 bytes do registro.
	* @return True quando o c�digo for inserido com sucesso ou, caso contr�rio, False.
	*/
	static bool				ACS_AddMasterCard( const byte code[] );

	/**
	* Acessa o campo de senha nas configura��es da controladora.
	* 
	* @return A senha de acesso da controladora.
	*/
	static uint32_t			ACS_GetPassword();
	
	/**
	* Sobrescreve o campo de senha administradora na configura��o da controladora.
	* 
	* @param password A nova senha administradora ser salva na controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				ACS_SetPassword( uint32_t password );
	
	/**
	* Faz uma busca nos registros salvos na controladora pelo {@code mifareID} e hidrata o objeto {@code reg}.
	* 
	* @param mifareID O ID de 4 bytes do registro.
	* @param reg O objeto a ser hidratado quando o registro for encontrado.
	* @return Retorna 1 quando o registro for encontrado ou, caso contr�rio, -1.
	*/
	static int				ACS_GetAccessRegister( const byte mifareID[4], AccessReg &reg );
#pragma endregion


/*
===============================================================================
Network settings.
===============================================================================
*/
#pragma region Network
	/**
	* Acessa o flag de DHCP nas configura��es da controladora.
	* 
	* @return True quando o DHCP for usado ou, caso contr�rio, False.
	*/
	static bool				NW_getIsDHCP();
	/**
	* Sobrescreve o flag para usar DHCP na configura��o da controladora.
	* 
	* @param value O novo flag a ser salvo na controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				NW_setIsDHCP( bool value );
	
	static bool				NW_loadIpAddressInto(byte target[4]);

	/**
	* Sobrescreve o IP usado pela controladora.
	* 
	* @param value O novo IP da controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				NW_setIpAddress( const byte value[4] );
	
	/**
	* Acessa a m�scara de rede usada pela controladora.
	* 
	* @return A m�scara de rede usada pela controladora.
	*/
	static bool				NW_loadMaskInto( byte target[4] );
	/**
	* Sobrescreve a m�scara de rede usada pela controladora.
	* 
	* @param value A nova m�scara de rede da controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				NW_setMask( const byte value[4] );

	/**
	* Acessa o endere�o IP do gateway usado pela controladora.
	* 
	* @return O endere�o IP do gateway usado pela controladora.
	*/
	static bool				NW_loadGatewayInto( byte target[4] );
	/**
	* Sobrescreve o endere�o IP do gateway usado pela controladora.
	* 
	* @param value O novo endere�o IP do gateway da controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				NW_setGateway( const byte value[4] );

	/**
	* Acessa o n�mero de terminal usado pela controladora.
	* 
	* @return O n�mero de terminal usado pela controladora.
	*/
	static uint8_t			NW_getTerminalNumber();
	/**
	* Sobrescreve o n�mero de terminal usado pela controladora.
	* 
	* @param value O novo n�mero de terminal da controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				NW_setTerminalNumber( uint8_t value );
#pragma endregion


/*
===============================================================================
Server settings.
===============================================================================
*/
#pragma region Server 
	/**
	* Busca o endere�o IP do servidor de acesso e o clona no {@code target} dado por par�metro.
	* 
	* @param target O endere�o a ser usado para clonar o endere�o IP do servidor de acesso.
	* @return True quando a opera��o for realizada com sucesso ou, caso contr�rio, False.
	*/
	static bool				SRV_loadIpAddressInto( byte target[4] );

	/**
	* Acessa o endere�o IP do servidor usado pela controladora.
	* 
	* @return O endere�o IP do servidor usado pela controladora.
	*/
	static uint32_t			SRV_getIpAddress();
	/**
	* Sobrescreve o endere�o IP do servidor usado pela controladora.
	* 
	* @param value O novo endere�o IP do servidor da controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				SRV_setIpAddress( const byte value[4] );

	/**
	* Acessa a porta do servidor usada pela controladora.
	* 
	* @return A porta do servidor usada pela controladora.
	*/
	static uint16_t			SRV_getPort();
	/**
	* Sobrescreve a porta do servidor usada pela controladora.
	* 
	* @param value A porta do servidor usada pela controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				SRV_setPort( uint16_t value );

	/**
	* Acessa o n�mero do computador de acesso.
	* 
	* @return O n�mero do computador de acesso, um inteiro entre 0 e 99.
	*/
	static uint8_t			SRV_getComputer();
	/**
	* Sobrescreve o n�mero do computador de acesso.
	* 
	* @param value O n�mero do computador de acesso, um inteiro que deve estar entre 0 e 99.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				SRV_setComputer( uint8_t value );
#pragma endregion


/*
===============================================================================
Actuation settings.
===============================================================================
*/
#pragma region Actuation 
	/**
	* Acessa o tipo de acionamento usado pela controladora.
	* 
	* @return True, quando for tipo de acionamento Aberto ou, quando for tipo de acionamento Fechado, retorna False.
	*/
	static bool				ACT_getType();
	/**
	* Sobrescreve o tipo de acionamento usado pela controladora.
	* 
	* @param value Novo valor de tipo de acionamento. True, quando for tipo Aberto ou, quando for tipo Fechado, False.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				ACT_setType( bool value );

	/**
	* Acessa o tempo de libera��o de acesso usado pela controladora.
	* 
	* @return O tempo, em millisegundos, que a controladora libera acesso quando requisitado.
	*/
	static uint16_t			ACT_getTime();
	/**
	* Sobrescreve o tempo de libera��o de acesso usado pela controladora.
	* 
	* @param value Novo valor, em millisegundos, do tempo de libera��o de acesso usado pela controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				ACT_setTime( uint16_t value );

	/**
	* Libera a porta e pausa a aplica��o pelo tempo determinado em System::setTime(uint16_t).
	*/
	static void				FreeGate();
#pragma endregion
	

/*
===============================================================================
DateTime settings.
===============================================================================
*/
#pragma region DateTime 
	/**
	* Busca o endere�o IP do servidor NTP e o clona no {@code target} dado por par�metro.
	* 
	* @param target O endere�o a ser usado para clonar o endere�o IP do servidor de acesso.
	* @return True quando a opera��o for realizada com sucesso ou, caso contr�rio, False.
	*/
	static bool				DT_loadNTPIpAddressInto( byte target[4] );

	/**
	* Acessa a data usada pelo {@code rtc}.
	* 
	* @param rtc M�dulo a ser verificado
	* @return A data usada pelo {@code rtc}.
	*/
	static DateTime			DT_getDateTime( RTC_DS1307 *rtc );
	/**
	* Sobrescreve a data usada pelo {@code rtc}.
	* 
	* @param rtc M�dulo a ser atualizado.
	* @param value Nova data a ser usada pelo {@code rtc}.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				DT_setDateTime( DateTime value, RTC_DS1307 *rtc );

	/**
	* Acessa o flag de uso se servidor NTP nas configura��es da controladora.
	* 
	* @return True quando o NTP for usado ou, caso contr�rio, False.
	*/
	static bool				DT_getUseNTP();
	/**
	* Sobrescreve o flag para atualizar a hora por servidor NTP na configura��o da controladora.
	* 
	* @param value O novo flag a ser salvo na controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				DT_setUseNTP( bool value );
	
	/**
	* Acessa o endere�o IP do servidor NTP usado pela controladora.
	* 
	* @return O endere�o IP do servidor NTP usado pela controladora.
	*/
	static uint32_t			DT_getNTPIpAddress();
	/**
	* Sobrescreve o endere�o IP do servidor NTP usado pela controladora.
	* 
	* @param ip O endere�o IP do servidor NTP.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				DT_setNTPIpAddress( const byte ip[4] );
#pragma endregion

	/**
	* Verifica se o terminal j� foi configurado.
	* 
	* @return True quando o terminal estiver configurado ou, caso contr�rio, False.
	*/
	static bool				GetIsSetup();

	/**
	* Define o estado do terminal como configurado.
	* 
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				SetIsSetup();

	/**
	* Limpa todos os registros de acesso gravados em mem�ria cache do terminal.
	* 
	* @return True quando o cache for limpo com sucesso ou, caso contr�rio, False.
	*/
	static bool				ClearRegistersCache();

private:

	static void				CloseGate();
	static void				OpenGate();

	static bool				readIpHelper( byte target[4], int8_t offset );

	static bool				writeIpHelper( const byte value[4], int8_t offset );

	/**
	* Faz a leitura de um n�mero inteiro de 32 bits
	* 
	* @param address O endere�o a realizar a leitura.
	* @return O n�mero de 32 bits lido pela controladora.
	*/
	static uint32_t			getUInt32Helper( int8_t address );
	/**
	* Sobrescreve o endere�o {@code address} com o n�mero de 32 bits {@code value}.
	* 
	* @param value O valor a ser gravado.
	* @return True quando o valor for atualizado com sucesso ou, caso contr�rio, False.
	*/
	static bool				setUInt32Helper( uint32_t value, int8_t address );
};

#endif /* SYSTEM_H */

#pragma warning( pop ) 