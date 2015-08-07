#include "MenuPanelView.h"
#include "System.h"
#include "IIDProvider.h"
#include "RTClib.h"

typedef struct {
	const char						*text;
	void							( *ok_func )( MenuPanelView* );
	void							( *back_func )( void );
} MenuOption;

MenuOption							*activatedMenuOption;
int8_t								currentMenuIndex;
int8_t								optionsCount;
bool								isInMenu;

#pragma region Method Stubs
void								gotoRootMenu();
void								closeMenu();
void								setActivatedMenuOption( MenuOption*, int8_t );

void								gotoAcesso		( MenuPanelView* );
void								gotoDataHora	( MenuPanelView* );
void								gotoRede		( MenuPanelView* );
void								gotoServidor	( MenuPanelView* );
void								gotoAcionamento ( MenuPanelView* );

void								okMenuAcessoAdicionarCartao ( MenuPanelView* );
void								okMenuAcessoRevogarCartao	( MenuPanelView* );
void								okMenuAcessoAdicionarMestre	( MenuPanelView* );
void								okMenuAcessoAlterarSenha	( MenuPanelView* );
void								AlterarSenha				( MenuPanelView*, bool );

void								okMenuDataHoraAjustarData ( MenuPanelView* );
void								okMenuDataHoraHorarioVerao( MenuPanelView* );
void								okMenuDataHoraFusoHorario ( MenuPanelView* );
void								okMenuDataHoraServidorNTP ( MenuPanelView* );
void								okMenuDataHoraUsarNTP	  ( MenuPanelView* );

bool								checkDHCP				 ( MenuPanelView* );
void								okMenuRedeDHCP			 ( MenuPanelView* );
void								okMenuRedeEnderecoIP	 ( MenuPanelView* );
void								okMenuRedeMascara		 ( MenuPanelView* );
void								okMenuRedeGateway		 ( MenuPanelView* );
void								okMenuRedeNumeroTerminal ( MenuPanelView* );

void								okMenuServidorEnderecoIP ( MenuPanelView* );
void								okMenuServidorPorta		 ( MenuPanelView* );
void								okMenuServidorComputador ( MenuPanelView* );

void								okMenuAcionamentoTipo  ( MenuPanelView* );
void								okMenuAcionamentoTempo ( MenuPanelView* );

bool								editTemplateFlag	  ( const __FlashStringHelper*, String&, String&, bool, MenuPanelView* );
uint8_t								editTemplateOptionList( const __FlashStringHelper *[], int , uint8_t , MenuPanelView* );
uint8_t								editTemplateUInt8	  ( const __FlashStringHelper*, uint8_t, MenuPanelView* );
uint16_t							editTemplateUInt16	  ( const __FlashStringHelper*, uint16_t, MenuPanelView* );
uint32_t							editTemplateIP		  ( const __FlashStringHelper*, byte[4], MenuPanelView* );
DateTime							editTemplateDateTime  ( const __FlashStringHelper*, DateTime, MenuPanelView* );
uint32_t							editTemplateUInt32	  ( const __FlashStringHelper*, uint32_t, MenuPanelView* );
uint32_t							editTemplatePassword  ( const __FlashStringHelper*, MenuPanelView* );
int									editTemplateMfrID	  ( byte&, MenuPanelView* );
#pragma endregion

#pragma region MenuOption Array Assignments
MenuOption innerMenuAcesso[] = {
	{ "Adicionar Cartao",			okMenuAcessoAdicionarCartao,			gotoRootMenu },
	{ "Revogar Cartao",				okMenuAcessoRevogarCartao,				gotoRootMenu },
	{ "Adicionar Mestre",			okMenuAcessoAdicionarMestre,			gotoRootMenu },
	{ "Alterar Senha",				okMenuAcessoAlterarSenha,				gotoRootMenu }
};

MenuOption innerMenuDataHora[] = {
	{ "Ajustar Data",				okMenuDataHoraAjustarData,				gotoRootMenu }, 
	{ "Horario de Verao",			okMenuDataHoraHorarioVerao,				gotoRootMenu }, 
	{ "Fuso horario",				okMenuDataHoraFusoHorario,				gotoRootMenu }, 
	{ "Usar NTP",					okMenuDataHoraUsarNTP,					gotoRootMenu },
	{ "Servidor NTP",				okMenuDataHoraServidorNTP,				gotoRootMenu }  
};														  
														  
MenuOption innerMenuRede[] = {							  
	{ "Numero terminal",			okMenuRedeNumeroTerminal,				gotoRootMenu }, 
	{ "DHCP",						okMenuRedeDHCP,							gotoRootMenu }, 
	{ "Endereco IP",				okMenuRedeEnderecoIP,					gotoRootMenu }, 
	{ "Mascara",					okMenuRedeMascara,						gotoRootMenu }, 
	{ "Gateway",					okMenuRedeGateway,						gotoRootMenu }  
};

MenuOption innerMenuServidor[] = {
	{ "Endereco IP",				okMenuServidorEnderecoIP,				gotoRootMenu }, 
	{ "Porta",						okMenuServidorPorta,					gotoRootMenu }, 
	{ "Computador",					okMenuServidorComputador,				gotoRootMenu }
};

MenuOption innerMenuAcionamento[] = {
	{ "Tipo",						okMenuAcionamentoTipo,					gotoRootMenu }, 
	{ "Tempo (milisseg)",			okMenuAcionamentoTempo,					gotoRootMenu }, 
};

MenuOption rootMenu[] = {
	{ "Acesso",						gotoAcesso,								closeMenu },
	{ "Data/Hora",					gotoDataHora,							closeMenu },
	{ "Rede",						gotoRede,								closeMenu },
	{ "Servidor",					gotoServidor,							closeMenu },
	{ "Acionamento",				gotoAcionamento,						closeMenu },
};
#pragma endregion

#pragma region Goto
void gotoAcesso( MenuPanelView *mpv ) {
	setActivatedMenuOption( innerMenuAcesso, 4 );
}
void gotoDataHora( MenuPanelView *mpv ) {
	setActivatedMenuOption( innerMenuDataHora, 5 );
}
void gotoRede( MenuPanelView *mpv ) {
	setActivatedMenuOption( innerMenuRede, 5 );
}
void gotoServidor( MenuPanelView *mpv ) {
	setActivatedMenuOption( innerMenuServidor, 2 );
}
void gotoAcionamento( MenuPanelView *mpv ) {
	setActivatedMenuOption( innerMenuAcionamento, 2 );
}
void gotoRootMenu() {
	setActivatedMenuOption( rootMenu, 5 );
}
void closeMenu() {
	isInMenu = false;
}
void setActivatedMenuOption( MenuOption *value, int8_t valueOptionsCount ) {
	activatedMenuOption = value;
	currentMenuIndex = 0;
	optionsCount = valueOptionsCount;
}
#pragma endregion

#pragma region Access Menu
void okMenuAcessoAdicionarCartao( MenuPanelView *mpv ) {
	byte id[ 4 ];

	int isCardRead = editTemplateMfrID( *id, mpv );
	mpv->lcd->clear();

	if ( isCardRead > 0 ) {
		AccessReg result;
		for ( int i = 0; i < 4; i++ )
			result.mifareID[i] = id[i];

		result.isMaster = false;
		result.allowedDaysOfWeek = B1111111;
		result.hourEnd = 23;
		result.hourStart = 0;
		result.untilDay = 31;
		result.untilMon = 12;
		result.untilYear = 2200;

		bool saved = System::ACS_AddAccessReg( result ) > 0;

		if ( saved ){
			System::LED_GREEN_ON();
			mpv->lcd->print(F("Acesso permitido"));
		}
		else {
			System::LED_RED_ON();
			mpv->lcd->print(F("  Sem memoria"));
		}

		mpv->lcd->setCursor(0, 1);

		for (int i = 0; i < 4; i++)
			mpv->lcd->print(id[i], HEX);
	}
	else {
		System::LED_RED_ON();
		mpv->lcd->println( F( "Nenhum cartao lido" ) );
	}

	delay( 2000 );
	System::LED_ALL_OFF();
}
void okMenuAcessoRevogarCartao( MenuPanelView *mpv ) {
	byte id[4];
	int isCardRead = editTemplateMfrID( *id, mpv );

	bool accessRevoked = System::ACS_RevokeCard( id );
	mpv->lcd->clear();

	if ( isCardRead < 0 )
		return;

	if ( accessRevoked ) {
		mpv->lcd->print( F( "Cartao revogado" ) );
		delay( 2000 );
	}
	else {
		mpv->lcd->print( F( "   Cartao nao" ) );
		mpv->lcd->setCursor( 0, 1 );
		mpv->lcd->print( F( "   encontrado" ) );
		System::NOTIFY_ERROR();
	}
}
void okMenuAcessoAdicionarMestre( MenuPanelView *mpv ) {
	byte id[4];

	int isCardRead = editTemplateMfrID( *id, mpv );
	mpv->lcd->clear();

	
	if ( isCardRead < 0 ) {
		mpv->lcd->print( F( "Nenhum cartao lido" ) );
		delay( 2000 );
	}

	AccessReg result;
	bool regExists = System::ACS_GetAccessRegister( id, result ) > 0;

	if ( regExists ){
		mpv->lcd->print( F( "   Cartao ja" ) );
		mpv->lcd->setCursor( 0, 1 );
		mpv->lcd->print( F( "   cadastrado" ) );

		delay( 2000 );
	}
	else if ( System::ACS_AddMasterCard( id ) ) {
		mpv->lcd->print( F( "     Mestre" ) );
		mpv->lcd->setCursor( 0, 1 );
		mpv->lcd->print( F( "   adicionado" ) );
	}
	else {
		mpv->lcd->print( F( "     Espaco" ) );
		mpv->lcd->setCursor( 0, 1 );
		mpv->lcd->print( F( "  insuficiente" ) );
	}
	

	delay( 2000 );
}
void okMenuAcessoAlterarSenha(MenuPanelView *mpv) {
	AlterarSenha( mpv, false );
}
void AlterarSenha( MenuPanelView *mpv, bool forceNewPassword ) {
	bool isActualPasswordCorrect = forceNewPassword || mpv->CheckPassword();
	mpv->lcd->clear();
	
	if ( !isActualPasswordCorrect ) {
		mpv->lcd->print( F ( "   Senha atual" ) );
		mpv->lcd->setCursor( 0, 1 );
		mpv->lcd->print( F ( "    incorreta" ) );

		System::NOTIFY_ERROR();
		return;
	}

	uint32_t newPassword = INVALID_PASSWORD;
	while (true) {
		newPassword = editTemplatePassword( F( "Nova senha:" ), mpv );
		
		while ( newPassword == INVALID_PASSWORD ) {
			mpv->lcd->print( F( " Senha invalida" ) );
			System::NOTIFY_ERROR();
		
			if ( forceNewPassword )
				newPassword = editTemplatePassword( F( "Nova senha:" ), mpv );
			else
				return;
		}

		mpv->lcd->clear();

		uint32_t newPasswordAgain = editTemplatePassword( F( "Repita nva senha" ), mpv );
		mpv->lcd->clear();
	
		if ( newPassword != newPasswordAgain ) {
			mpv->lcd->print( F( "     Senhas" ) );
			mpv->lcd->setCursor( 0, 1 );
			mpv->lcd->print( F( "   Diferentes" ) );

			System::NOTIFY_ERROR();
		
			if (!forceNewPassword)
				return;
		}
		else
			break;
	}
	
	System::ACS_SetPassword( newPassword );

	mpv->lcd->print( F( " Senha alterada" ) );
	mpv->lcd->setCursor( 0, 1 );
	mpv->lcd->print( F( "  com sucesso" ) );
	delay( 2000 );
}
#pragma endregion

#pragma region Date&Time Menu
void okMenuDataHoraAjustarData( MenuPanelView *mpv ) {
	if ( System::DT_getUseNTP() ) {

		mpv->lcd->clear();

		mpv->lcd->print( F( " Desabilite NTP" ) );
		mpv->lcd->setCursor( 0, 1 );
		mpv->lcd->print( F( "p mudanca manual" ) );

		System::NOTIFY_ERROR();

		return;
	}

	DateTime dateTime = System::DT_getDateTime( mpv->rtc );
	dateTime = DateTimeAdapter::ToLocalTime( dateTime );

	DateTime newdateTime = editTemplateDateTime( F( "Data e hora:" ), dateTime, mpv );
	newdateTime = DateTimeAdapter::ToUTC( newdateTime );

	System::DT_setDateTime( newdateTime, mpv->rtc );
}
void okMenuDataHoraHorarioVerao( MenuPanelView *mpv ) {
	bool ntpFlag = System::DT_getAutoDaylightSaving();
	String sSim = "Sim";
	String sNao = "Nao";

	bool newDST = editTemplateFlag( F( "Usar automatico:" ), sSim, sNao, ntpFlag, mpv );

	System::DT_setAutoDaylightSaving( newDST );
}
void okMenuDataHoraFusoHorario( MenuPanelView *mpv ) {
	static const __FlashStringHelper *OPTIONS[] = {
		F( "Noronha  UTC2" ),
		F( "Brasilia UTC3" ),
		F( "Amazonia UTC4" ),
		F( "Acre     UTC5" )
	};

	int countOptions = 4;
	uint8_t offset = System::DT_getTimeZoneOffset();

	uint8_t newOffset = editTemplateOptionList( OPTIONS, countOptions, offset - 2, mpv );
	
	newOffset += 2;

	System::DT_setTimeZoneOffset( newOffset );
}
void okMenuDataHoraServidorNTP( MenuPanelView *mpv ) {
	if ( !System::DT_getUseNTP() ) {
		mpv->lcd->clear();

		mpv->lcd->print( F( " Habilite NTP p/" ) );
		mpv->lcd->setCursor( 0, 1 );
		mpv->lcd->print( F( " alterar ip NTP" ) );

		System::NOTIFY_ERROR();

		return;
	}
	
	byte ntpIp[4];
	System::DT_loadNTPIpAddressInto( ntpIp );
	bool ipChanged = editTemplateIP( F( "IP Servidor NTP:" ), ntpIp, mpv ) > 0;

	if ( ipChanged )
		System::DT_setNTPIpAddress( ntpIp );
	
	mpv->lcd->clear();
	mpv->lcd->print( F( " Conectando..." ) );

	DateTime result;
	bool success = mpv->ntpDateTimeProvider->TryGetDateTime( result, false );

	if (success)
		System::DT_setDateTime( result, mpv->rtc );
	else {
		mpv->lcd->home();
		mpv->lcd->print( F( "Falha em conexao" ) );
		
		System::NOTIFY_ERROR();
	}
}
void okMenuDataHoraUsarNTP( MenuPanelView *mpv ) {
	bool ntpFlag = System::DT_getUseNTP();
	String sSim = "Sim";
	String sNao = "Nao";

	bool newDhcpFlag = editTemplateFlag( F( "Usar NTP:" ), sSim, sNao, ntpFlag, mpv );

	System::DT_setUseNTP( newDhcpFlag );
}
#pragma endregion

#pragma region Network Menu
void okMenuRedeDHCP( MenuPanelView *mpv ) {
	bool dhcpFlag = System::NW_getIsDHCP();
	String sSim = "Sim";
	String sNao = "Nao";

	bool newDhcpFlag = editTemplateFlag( F( "Usar DHCP:" ), sSim, sNao, dhcpFlag, mpv );

	System::NW_setIsDHCP(newDhcpFlag);
}
void okMenuRedeEnderecoIP( MenuPanelView *mpv ) {
	if ( checkDHCP( mpv ) )
		return;

	byte networkIp[4];
	System::NW_loadIpAddressInto( networkIp );
	bool ipChanged = editTemplateIP( F( "IP de Rede:" ), networkIp, mpv ) > 0;

	if ( ipChanged )
		System::NW_setIpAddress( networkIp );
}
void okMenuRedeMascara( MenuPanelView *mpv ) {
	if ( checkDHCP( mpv ) )
		return;

	byte mask[4];
	System::NW_loadMaskInto( mask );
	bool ipChanged = editTemplateIP( F( "Mascara de Rede:" ), mask, mpv ) > 0;
	
	if ( ipChanged )
		System::NW_setMask( mask );
}
void okMenuRedeGateway( MenuPanelView *mpv ) {
	if ( checkDHCP( mpv ) )
		return;

	byte gateway[4];
	System::NW_loadGatewayInto( gateway );
	bool ipChanged = editTemplateIP( F( "Gateway:" ), gateway, mpv ) > 0;
	
	if ( ipChanged )
		System::NW_setGateway( gateway );
}
void okMenuRedeNumeroTerminal( MenuPanelView *mpv ) {
	uint8_t terminal = System::NW_getTerminalNumber();
	uint8_t newTerminal = editTemplateUInt8( F( "Numero terminal:" ), terminal, mpv );
	
	System::NW_setTerminalNumber( newTerminal );
}

bool checkDHCP( MenuPanelView *mpv ) {
	if ( System::NW_getIsDHCP() ) {
		mpv->lcd->clear();
		
		mpv->lcd->print( F( "DHCP habilitado" ) );
		mpv->lcd->setCursor( 0, 1 );
		mpv->lcd->print( F( "opc n disponivel" ) );

		System::NOTIFY_ERROR();

		return true;
	}
	return false;
}
#pragma endregion

#pragma region ServerMenu
void okMenuServidorEnderecoIP( MenuPanelView *mpv ) {
	byte serverIp[4];
	System::SRV_loadIpAddressInto( serverIp );
	bool ipChanged = editTemplateIP( F( "IP do Servidor:" ), serverIp, mpv ) > 0;

	if ( ipChanged )
		System::SRV_setIpAddress( serverIp );
}
void okMenuServidorPorta( MenuPanelView *mpv ) {
	uint16_t port = System::SRV_getPort();
	uint16_t newPort = editTemplateUInt16( F( "Porta Servidor:" ), port, mpv );
	
	System::SRV_setPort( newPort );
}
void okMenuServidorComputador( MenuPanelView *mpv ) {
	uint8_t computer = System::SRV_getComputer();
	uint8_t newComputer = editTemplateUInt8( F( "Computador:" ), computer, mpv );
	
	System::SRV_setComputer( newComputer );
}
#pragma endregion

#pragma region Actuation Menu 
void okMenuAcionamentoTipo( MenuPanelView *mpv ) {
	bool actFlag = System::ACT_getType();
	String sAberto = "Abrt";
	String sFechado = "Fechd";
	bool newActFlag = editTemplateFlag( F( "Tipo Acionamento" ), sAberto, sFechado, actFlag, mpv );
	
	System::ACT_setType(newActFlag);
}
void okMenuAcionamentoTempo( MenuPanelView *mpv ) {
	uint16_t port = System::ACT_getTime();
	uint16_t newPort = editTemplateUInt16( F("Tempo(milliseg):"), port, mpv );
	
	System::ACT_setTime( newPort );
}
#pragma endregion

#pragma region EditorTemplates

const char *bktsUnchecked = "[ ]";
const char *bktsChecked = "[*]";

#define getBkstFor(value) value ? bktsChecked : bktsUnchecked

//TODO: const const T[]
uint8_t editTemplateOptionList( const __FlashStringHelper *options[],
								int countOptions,
								uint8_t selectedItem,
								MenuPanelView *mpv ) {
	char c = '5';
	int page = 0, focused = 0, selected = selectedItem;
	int maxPage = ((countOptions + (countOptions % 2)) / 2) - 1;

	mpv->lcd->clear();
	mpv->lcd->cursor_on();

	do {
		switch (c) {
		case '2':
			if ( focused <= 0 )
				break;
			focused--;
			page = focused / 2;
			break;
		case '8':
			if ( focused >= countOptions-1 )
				break;
			focused++;
			page = focused / 2;
			break;
		case '*':
			selected = focused;
			break;
		default:
			break;
		}

		int optLine1 = page * 2;
		int optLine2 = optLine1 + 1;

		mpv->lcd->setCursor( 0, 0 );
		mpv->lcd->print( getBkstFor(optLine1 == selected) );
		mpv->lcd->print( options[optLine1] );

		if ( optLine2 < countOptions ) {
			mpv->lcd->setCursor( 0, 1 );
			mpv->lcd->print( getBkstFor(optLine2 == selected) );
			mpv->lcd->print( options[optLine2] );
		}

		mpv->lcd->setCursor(1, focused % 2);
		c = mpv->keyPadListener->WaitForInput();
	} while ( c != '#' );

	mpv->lcd->cursor_off();
	return selected;
}

boolean editTemplateFlag( const __FlashStringHelper *editorDisplay,
						  String& optFlagOn,
						  String& optFlagOff,
						  boolean current,
						  MenuPanelView *mpv ) {
	mpv->lcd->clear();
	mpv->lcd->print(editorDisplay);

	uint8_t lenOpts = optFlagOn.length() + optFlagOff.length();
	uint8_t lenDisplay = 16;
	uint8_t lenBrackets = 6;
	
	boolean result = current;
	char c = '5';
	do {
		switch (c) {
		case '2':
		case '8':
		case '6':
		case '4':
			result = !result;
			break;
		default:
			break;
		}

		mpv->lcd->setCursor( ( lenDisplay - ( lenOpts +  1 + lenBrackets ) ) / 2, 1 );
	
		mpv->lcd->print( result ? bktsChecked : bktsUnchecked );
		mpv->lcd->print( optFlagOn );
	
		mpv->lcd->print( ' ' );

		mpv->lcd->print( result ? bktsUnchecked : bktsChecked );
		mpv->lcd->print( optFlagOff );

		c = mpv->keyPadListener->WaitForInput();
	} while ( c != '#' && c != '*' );

	if ( c == '#' )
		return current;

	return result;	
}

uint8_t editTemplateUInt8( const __FlashStringHelper *editorDisplay, uint8_t current, MenuPanelView *mpv ) {
	mpv->lcd->clear();
	mpv->lcd->print( editorDisplay );
	
	uint8_t result;
	while ( true ) {
		mpv->lcd->setCursor( 7, 1 );
	
		if ( current < 10 )
			mpv->lcd->print('0');

		mpv->lcd->print( current, DEC );

		result = 0;

		mpv->lcd->cursor_on();
		for ( uint8_t i = 0; i < 2; i++ )
		{
			mpv->lcd->setCursor( 7+i, 1 );

			char newValue;
			do {
				newValue = mpv->keyPadListener->WaitForInput();
				if ( newValue == '#' ) {
					mpv->lcd->cursor_off();
					return current;
				}
			} while ( newValue < '0' || newValue > '9' );

			result = ( result * 10 ) + newValue - '0';
			mpv->lcd->print( newValue );
		}
		mpv->lcd->cursor_off();

		if ( result <= 99 )
			break;
	}

	return ( uint16_t )result;
}


uint16_t editTemplateUInt16( const __FlashStringHelper *editorDisplay, uint16_t current, MenuPanelView *mpv ) {
	mpv->lcd->clear();
	mpv->lcd->print( editorDisplay );

	uint32_t result;
	while ( true ) {
		mpv->lcd->setCursor( 5, 1 );
	
		uint16_t intLength = 10000;
		while ( current < intLength ) {
			mpv->lcd->print('0');
			intLength /= 10;
		}
		mpv->lcd->print( current, DEC );

		result = 0;

		mpv->lcd->cursor_on();
		for ( uint8_t i = 0; i < 5; i++ )
		{
			mpv->lcd->setCursor( 5+i, 1 );

			char newValue;
			do {
				newValue = mpv->keyPadListener->WaitForInput();
				if ( newValue == '#' ) {
					mpv->lcd->cursor_off();
					return current;
				}
			} while ( newValue < '0' || newValue > '9' );

			result = ( result * 10 ) + newValue - '0';
			mpv->lcd->print( newValue );
		}
		mpv->lcd->cursor_off();

		if ( result <= 65535 )
			break;
	}

	return ( uint16_t )result;
}

uint32_t editTemplateIP( const __FlashStringHelper * editorDisplay, byte current[4], MenuPanelView *mpv ) {
	uint16_t ip[4] = {
		current[0],
		current[1],
		current[2],
		current[3]
	};

	mpv->lcd->clear();
	mpv->lcd->print( editorDisplay );
	mpv->lcd->setCursor( 0, 1 );

	for ( int i = 0; i < 4; ) {
		if ( ip[i] < 100 )
			mpv->lcd->print( 0, DEC );
		
		if ( ip[i] < 10 )
			mpv->lcd->print( 0, DEC );

		mpv->lcd->print( ip[i], DEC );

		if ( ++i < 4 )
			mpv->lcd->print( '.' );
	}

	mpv->lcd->cursor_on();
	for ( int idxOctet = 0; idxOctet < 4; idxOctet++ ) {
		uint32_t octet = 0;
		for ( int i = 0; i < 3; i++ )
		{
			uint8_t posCursor = ( idxOctet * 4 ) + i;
			mpv->lcd->setCursor( posCursor, 1 );

			char newValue;
			do {
				newValue = mpv->keyPadListener->WaitForInput();
				if ( newValue == '#' ) {
					mpv->lcd->cursor_off();
					return -1;
				}
			} while ( newValue < '0' || newValue > '9' );

			octet = (octet * 10) + newValue - '0';

			mpv->lcd->print( newValue );
		}
		if ( octet > 255 ) {
			idxOctet--;
			continue;
		}

		ip[idxOctet] = octet;
	}
	mpv->lcd->cursor_off();

	for (int i = 0; i < 4; i++)
		current[i] = ip[i];

	return 1;
}

DateTime editTemplateDateTime( const __FlashStringHelper * editorDisplay, DateTime current, MenuPanelView *mpv )
{
#pragma warning( push )
#pragma warning( disable : Woverflow)
	uint8_t values[] = { 
		current.hour(),
		current.minute(),
		current.day(),
		current.month(),
		current.year() % 100 // As (2147483647 % 100) = 47 < 255, you can ignore narrowing conversion  warning
	};
#pragma warning( pop ) //Woverflow
	
	char separators[] = { ':', ' ', '/', '/', ' ' };
	uint8_t maxValues[] = { 23, 59, 31, 12, 99 };

	mpv->lcd->cursor_on();

	mpv->lcd->clear();
	mpv->lcd->print( editorDisplay );
	mpv->lcd->setCursor( 0, 1 );

	for ( int i = 0; i < 5; i++ ) {
		mpv->lcd->setCursor((i * 3) + 1, 1 );

		if ( values[i] < 10 )
			mpv->lcd->print( '0' );

		mpv->lcd->print( values[i], DEC );
		mpv->lcd->print( separators[i] );
	}

	char newValue;
	uint8_t idx = 0;

	while (true) {
		if ( idx > 4 )
			idx = 0;

		for ( int i = 0; i < 2; i++ )
		{
			mpv->lcd->setCursor( ( idx * 3 ) + 1 + i, 1 );
			newValue = mpv->keyPadListener->WaitForInput();

			if ( newValue == '#' ) {
				mpv->lcd->cursor_off();
				return current;
			}
			else if ( newValue == '*' ) {
				mpv->lcd->cursor_off();
				return DateTime( values[4] + 2000, values[3], values[2], values[0], values[1] );
			}

			mpv->lcd->print( newValue );

			if (i == 0) //Most important part: _0
				values[idx] = ( newValue - '0' ) * 10 + ( values[idx] % 10 );
			else        //Less important part: 0_
				values[idx] = values[idx] - ( values[idx] % 10 ) + ( newValue - '0' );
		}

		if (values[idx] <= maxValues[idx])
			idx++;
	}
}

int editTemplateMfrID( byte &value, MenuPanelView *mpv ) {
	String sDigit = "Digt";
	String sReader = "Cartao";
	
	boolean useKeyboard = editTemplateFlag( F( "Modo de entrada" ), sDigit, sReader, true, mpv );
	
	if (useKeyboard) {                      
		uint32_t id = editTemplateUInt32( F( "Digite o codigo:" ), 0, mpv );
		
		( &value )[0] = ( byte ) ( id >> 24);
		( &value )[1] = ( byte )(( id & 0x00FF0000 ) >> 16);
		( &value )[2] = ( byte )(( id & 0x0000FF00 ) >> 8);
		( &value )[3] = ( byte )(( id & 0x000000FF ));
	
		return 1;
	}
	else {
		mpv->lcd->clear();
		mpv->lcd->print( F( "Aprxime o cartao" ) );
		
		return mpv->idProvider->WaitForNextID( value, 10000 );
	}
}

uint32_t editTemplateUInt32( const __FlashStringHelper * editorDisplay, uint32_t current, MenuPanelView *mpv ) {
	mpv->lcd->clear();
	mpv->lcd->print( editorDisplay );

	uint64_t result;
	while ( true ) {
		mpv->lcd->setCursor( 3, 1 );

		uint32_t intLength = 1000000000;
		while ( current < intLength ) {
			mpv->lcd->print( '0' );
			intLength /= 10;
		}
		mpv->lcd->print( current, DEC );

		result = 0;

		mpv->lcd->cursor_on();
		for ( uint8_t i = 0; i < 10; i++ )
		{
			mpv->lcd->setCursor( 3+i, 1 );

			char newValue;
			do {
				newValue = mpv->keyPadListener->WaitForInput();
				if ( newValue == '#' ) {
					mpv->lcd->cursor_off();
					return current;
				}
			} while ( newValue < '0' || newValue > '9' );

			result = ( result * 10 ) + newValue - '0';
			mpv->lcd->print( newValue );
		}
		mpv->lcd->cursor_off();

		if ( result <= 2147483647 )
			break;
	}

	return ( uint32_t )result;
}

uint32_t editTemplatePassword(const __FlashStringHelper * editorDisplay, MenuPanelView *mpv) {
	
	mpv->lcd->clear();
	mpv->lcd->print( editorDisplay );

	uint64_t result;
	bool faulted = false;
	while ( true ) {
		if ( faulted ) {
			mpv->lcd->setCursor( 1, 1 );
			mpv->lcd->print( F("Senha invalida") );
			
			System::NOTIFY_ERROR();

			mpv->lcd->setCursor( 0, 1 );
			mpv->lcd->print("   ");
		}

		mpv->lcd->setCursor( 3, 1 );

		for (int i = 0; i < 10; i++)
			mpv->lcd->print( '_' );

		mpv->lcd->print("   ");
		result = 0;

		mpv->lcd->cursor_on();
		for ( uint8_t i = 0; i < 10; i++ )
		{
			mpv->lcd->setCursor( 3+i, 1 );

			char newValue;
			do {
				newValue = mpv->keyPadListener->WaitForInput();
				if ( newValue == '#' ) {
					mpv->lcd->cursor_off();
					return INVALID_PASSWORD;
				}
			} while ( newValue < '0' || newValue > '9' );

			result = ( result * 10 ) + newValue - '0';
			mpv->lcd->print( '*' );
		}
		mpv->lcd->cursor_off();

		if ( result <= 2147483647 && result != INVALID_PASSWORD)
			break;
		else
			faulted = true;
	}

	return ( uint32_t )result;
}

#pragma endregion

MenuPanelView::MenuPanelView( LiquidCrystal_I2C* lcd,
							  KeyPadListener* keyPad,
							  RTC_DS1307* rtc,
							  IIDProvider* idProvider,
							  IAccessRegWriter* accessRegWriter,
							  NTPDateTimeProvider* ntpDateTimeProvider )
	: ViewBase( lcd ) {
	this->hasNewFrame = isInMenu = false;
	this->keyPadListener = keyPad;
	this->rtc = rtc;
	this->idProvider = idProvider;
	this->accessRegWriter = accessRegWriter;
	this->ntpDateTimeProvider = ntpDateTimeProvider;
}

void MenuPanelView::Loop() {
	while ( isInMenu ) {
		Draw();
		ListenKeypad();
	}
}

void MenuPanelView::ListenKeypad() {
	char c = -1;

	c = this->keyPadListener->WaitForInput();
	switch (c)
	{
	case KEY_UP:
		TryToScroll( -1 );
		break;
	case KEY_DOWN:
		TryToScroll( 1 );
		break;
	case KEY_OK:
		activatedMenuOption[currentMenuIndex].ok_func( this );
		hasNewFrame = true;
		break;
	case KEY_CANCEL:
		activatedMenuOption[currentMenuIndex].back_func();
		hasNewFrame = true;
		break;
	default:
		delay( 250 );
		break;
	}

}

bool MenuPanelView::TryToScroll( int8_t delta ) {
	if ( currentMenuIndex + delta < 0 )
		return false;

	if ( currentMenuIndex + delta >= optionsCount )
		return false;

	currentMenuIndex += delta;
	hasNewFrame = true;
	delay( 500 );
	return true;
}

bool MenuPanelView::CheckPassword() {
	uint32_t entryPassword = editTemplatePassword( F( "Senha de acesso:" ), this );
	return entryPassword == System::ACS_GetPassword();
}

void MenuPanelView::Active() {
	bool hasMenuAccess = CheckPassword();

	if ( hasMenuAccess ) {
		gotoRootMenu();
		this->hasNewFrame = isInMenu = true;
	}
	else {
		lcd->clear();
		lcd->print(F("Senha incorreta"));

		System::NOTIFY_ERROR();
	}
}

void MenuPanelView::OnDraw() {
	this->lcd->clear();

	this->lcd->setCursor( 0, 0 );
	this->lcd->print( activatedMenuOption[currentMenuIndex].text );

	this->lcd->setCursor( 0, 1 );
	this->lcd->print( F( "*Y#N 2" ) );
	this->lcd->write( byte( UP_ARROW_IDX ) );
	
	this->lcd->print( F( " 8") );
	this->lcd->write( byte( DOWN_ARROW_IDX ) );

	this->lcd->print( F( " 6" ) );
	this->lcd->write( byte( RIGHT_ARROW_IDX ) );
	
	this->lcd->print( F( " 4" ) );
	this->lcd->write( byte( LEFT_ARROW_IDX ) );
}

bool MenuPanelView::HasNewFrame() {
	if ( hasNewFrame ) {
		hasNewFrame = false;
		return true;
	}
	return false;
}

void MenuPanelView::RunSetupIfNeeded() {
	if (System::GetIsSetup())
		return;

	//Limpamos qualquer sujeira que esteja na memória
	System::Reset();

	AlterarSenha( this, true );

	okMenuDataHoraAjustarData(this);
	okMenuRedeNumeroTerminal(this);
	okMenuServidorEnderecoIP(this);
	okMenuServidorPorta(this);

	okMenuRedeDHCP(this);
	if ( !System::NW_getIsDHCP() ) {
		okMenuRedeMascara(this);
		okMenuRedeEnderecoIP(this);
	}

	okMenuAcionamentoTipo(this);
	okMenuAcionamentoTempo(this);

	System::SetIsSetup();
}

MenuPanelView::~MenuPanelView() { }