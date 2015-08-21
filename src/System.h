#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <Ethernet.h>

#include "utils.h"
#include "AccessReg.h"
#include "IAccessRegWriter.h"

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define MAX_EEPROM_SIZE		  4096
#else
#define MAX_EEPROM_SIZE		  1024
#endif

//Deve ser sempre o primeiro índice da memória EEPROM (Por conta do System::Reset())
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
#define OFFSET_DT_DaylightSaving 34
#define OFFSET_DT_TimeZone		 35
//Deve ser sempre o último endereço usado na memória EEPROM (Por conta do System::Reset())
#define OFFSET_ACS_Regs			 36

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
	* Liga o led vermelho, pausa a execução do programa e então desliga o led.
	* 
	* @param duration O tempo, em millisegundos, para pausar a aplicação.
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
	* Liga o led verde, pausa a execução do programa e então desliga o led.
	* 
	* @param duration O tempo, em millisegundos, para pausar a aplicação.
	*/
	static void				LED_GREEN_BLINK( unsigned long duration = 100 );
	
	/**
	* Liga o buzzer, pausa a execução do programa e então desliga o buzzer.
	* 
	* @param duration O tempo, em millisegundos, para pausar a aplicação.
	*/
	static void				BEEP( unsigned long duration = 100 );

	/**
	* Liga o led vermelho, faz 3 BEEPs e pausa a aplicação
	*
	* @param duration O tempo, em millisegundos, para pausar a aplicação.
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
	* Remove o acesso do registro com o código {@code code} da memória interna da controladora.
	* 
	* @param code O ID de 4 bytes do registro.
	* @return True quando o registro for removido com sucesso ou, caso contrário, False.
	*/
	static bool				ACS_RevokeCard( const byte code[] );
	
	/**
	* Grava o registro na memória interna da controladora.
	* 
	* @param value O registro a ser salvo na memória interna da controladora.
	* @return True quando o registro for inserido com sucesso ou, caso contrário, False.
	*/
	static bool				ACS_AddAccessReg( const AccessReg &value );
		
	/**
	* Adiciona o código {@code card} na lista de códigos administradores da controladora.
	* 
	* @param card O ID de 4 bytes do registro.
	* @return True quando o código for inserido com sucesso ou, caso contrário, False.
	*/
	static bool				ACS_AddMasterCard( const byte code[] );

	/**
	* Acessa o campo de senha nas configurações da controladora.
	* 
	* @return A senha de acesso da controladora.
	*/
	static uint32_t			ACS_GetPassword();
	
	/**
	* Sobrescreve o campo de senha administradora na configuração da controladora.
	* 
	* @param password A nova senha administradora ser salva na controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				ACS_SetPassword( uint32_t password );
	
	/**
	* Faz uma busca nos registros salvos na controladora pelo {@code mifareID} e hidrata o objeto {@code reg}.
	* 
	* @param mifareID O ID de 4 bytes do registro.
	* @param reg O objeto a ser hidratado quando o registro for encontrado.
	* @return Retorna 1 quando o registro for encontrado ou, caso contrário, -1.
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
	* Acessa o flag de DHCP nas configurações da controladora.
	* 
	* @return True quando o DHCP for usado ou, caso contrário, False.
	*/
	static bool				NW_getIsDHCP();
	/**
	* Sobrescreve o flag para usar DHCP na configuração da controladora.
	* 
	* @param value O novo flag a ser salvo na controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				NW_setIsDHCP( bool value );
	
	static bool				NW_loadIpAddressInto(byte target[4]);

	/**
	* Sobrescreve o IP usado pela controladora.
	* 
	* @param value O novo IP da controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				NW_setIpAddress( const byte value[4] );
	
	/**
	* Acessa a máscara de rede usada pela controladora.
	* 
	* @return A máscara de rede usada pela controladora.
	*/
	static bool				NW_loadMaskInto( byte target[4] );
	/**
	* Sobrescreve a máscara de rede usada pela controladora.
	* 
	* @param value A nova máscara de rede da controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				NW_setMask( const byte value[4] );

	/**
	* Acessa o endereço IP do gateway usado pela controladora.
	* 
	* @return O endereço IP do gateway usado pela controladora.
	*/
	static bool				NW_loadGatewayInto( byte target[4] );
	/**
	* Sobrescreve o endereço IP do gateway usado pela controladora.
	* 
	* @param value O novo endereço IP do gateway da controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				NW_setGateway( const byte value[4] );

	/**
	* Acessa o número de terminal usado pela controladora.
	* 
	* @return O número de terminal usado pela controladora.
	*/
	static uint8_t			NW_getTerminalNumber();
	/**
	* Sobrescreve o número de terminal usado pela controladora.
	* 
	* @param value O novo número de terminal da controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
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
	* Busca o endereço IP do servidor de acesso e o clona no {@code target} dado por parâmetro.
	* 
	* @param target O endereço a ser usado para clonar o endereço IP do servidor de acesso.
	* @return True quando a operação for realizada com sucesso ou, caso contrário, False.
	*/
	static bool				SRV_loadIpAddressInto( byte target[4] );

	/**
	* Acessa o endereço IP do servidor usado pela controladora.
	* 
	* @return O endereço IP do servidor usado pela controladora.
	*/
	static uint32_t			SRV_getIpAddress();
	/**
	* Sobrescreve o endereço IP do servidor usado pela controladora.
	* 
	* @param value O novo endereço IP do servidor da controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
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
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				SRV_setPort( uint16_t value );

	/**
	* Acessa o número do computador de acesso.
	* 
	* @return O número do computador de acesso, um inteiro entre 0 e 99.
	*/
	static uint8_t			SRV_getComputer();
	/**
	* Sobrescreve o número do computador de acesso.
	* 
	* @param value O número do computador de acesso, um inteiro que deve estar entre 0 e 99.
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
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
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				ACT_setType( bool value );

	/**
	* Acessa o tempo de liberação de acesso usado pela controladora.
	* 
	* @return O tempo, em millisegundos, que a controladora libera acesso quando requisitado.
	*/
	static uint16_t			ACT_getTime();
	/**
	* Sobrescreve o tempo de liberação de acesso usado pela controladora.
	* 
	* @param value Novo valor, em millisegundos, do tempo de liberação de acesso usado pela controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				ACT_setTime( uint16_t value );

	/**
	* Libera a porta e pausa a aplicação pelo tempo determinado em System::setTime(uint16_t).
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
	* Busca o endereço IP do servidor NTP e o clona no {@code target} dado por parâmetro.
	* 
	* @param target O endereço a ser usado para clonar o endereço IP do servidor de acesso.
	* @return True quando a operação for realizada com sucesso ou, caso contrário, False.
	*/
	static bool				DT_loadNTPIpAddressInto( byte target[4] );

	/**
	* Acessa a data usada pelo {@code rtc}.
	* 
	* @param rtc Módulo a ser verificado
	* @return A data usada pelo {@code rtc}.
	*/
	static DateTime			DT_getDateTime( RTC_DS1307 *rtc );
	/**
	* Sobrescreve a data usada pelo {@code rtc}.
	* 
	* @param rtc Módulo a ser atualizado.
	* @param value Nova data a ser usada pelo {@code rtc}.
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				DT_setDateTime( DateTime value, RTC_DS1307 *rtc );

	/**
	* Acessa o flag de uso se servidor NTP nas configurações da controladora.
	* 
	* @return True quando o NTP for usado ou, caso contrário, False.
	*/
	static bool				DT_getUseNTP();
	/**
	* Sobrescreve o flag para atualizar a hora por servidor NTP na configuração da controladora.
	* 
	* @param value O novo flag a ser salvo na controladora.
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				DT_setUseNTP( bool value );

	/**
	* Acessa o campo de cálculo do horário de verão automático.
	* 
	* @return Retorna o modo de horário de verão usado pela controladora:
	*			   0 - automático;
	*			   1 - dentro do horário de verão,
	*			   2 - fora do horário de verão
	*/
	static uint8_t				DT_getAutoDaylightSaving();
	/**
	* Sobrescreve o campo do modo de horário de verão.
	* 
	* @param value O novo valor a ser salvo na controladora. O valor deve ser:
	*			   0 - automático;
	*			   1 - dentro do horário de verão,
	*			   2 - fora do horário de verão
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				DT_setAutoDaylightSaving( uint8_t value );

	/**
	* UNDONE doc
	*/
	static int8_t			DT_getTimeZoneOffset();
	/**
	* UNDONE doc
	*/
	static bool				DT_setTimeZoneOffset( int8_t value );

	
	/**
	* Acessa o endereço IP do servidor NTP usado pela controladora.
	* 
	* @return O endereço IP do servidor NTP usado pela controladora.
	*/
	static uint32_t			DT_getNTPIpAddress();
	/**
	* Sobrescreve o endereço IP do servidor NTP usado pela controladora.
	* 
	* @param ip O endereço IP do servidor NTP.
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				DT_setNTPIpAddress( const byte ip[4] );
#pragma endregion

	/**
	* Verifica se o terminal já foi configurado.
	* 
	* @return True quando o terminal estiver configurado ou, caso contrário, False.
	*/
	static bool				GetIsSetup();

	/**
	* Define o estado do terminal como configurado.
	* 
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				SetIsSetup();

	/**
	* Zera todas as configurações do terminal e limpa todos os registros de acesso gravados.
	* 
	* @return True quando o Reset for realizado com sucesso ou, caso contrário, False.
	*/
	static bool				Reset();

private:

	static void				CloseGate();
	static void				OpenGate();

	static bool				readIpHelper( byte target[4], int8_t offset );

	static bool				writeIpHelper( const byte value[4], int8_t offset );

	/**
	* Faz a leitura de um número inteiro de 32 bits
	* 
	* @param address O endereço a realizar a leitura.
	* @return O número de 32 bits lido pela controladora.
	*/
	static uint32_t			getUInt32Helper( int8_t address );
	/**
	* Sobrescreve o endereço {@code address} com o número de 32 bits {@code value}.
	* 
	* @param value O valor a ser gravado.
	* @return True quando o valor for atualizado com sucesso ou, caso contrário, False.
	*/
	static bool				setUInt32Helper( uint32_t value, int8_t address );
};

#endif /* SYSTEM_H */

#pragma warning( pop ) 