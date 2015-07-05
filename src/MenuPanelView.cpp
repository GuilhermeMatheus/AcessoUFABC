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
void								okMenuAcessoRevogarMestre	( MenuPanelView* );
void								okMenuAcessoAlterarSenha	( MenuPanelView* );

void								okMenuDataHoraAjustarData ( MenuPanelView* );
void								okMenuDataHoraServidorNTP ( MenuPanelView* );

void								okMenuRedeDHCP		 ( MenuPanelView* );
void								okMenuRedeEnderecoIP ( MenuPanelView* );
void								okMenuRedeMascara	 ( MenuPanelView* );
void								okMenuRedeGateway	 ( MenuPanelView* );

void								okMenuServidorEnderecoIP ( MenuPanelView* );
void								okMenuServidorPorta		 ( MenuPanelView* );

void								okMenuAcionamentoTipo  ( MenuPanelView* );
void								okMenuAcionamentoTempo ( MenuPanelView* );

bool								editTemplateFlag	 ( const __FlashStringHelper*, String&, String&, bool, MenuPanelView*);
uint16_t							editTemplateUInt16	 ( const __FlashStringHelper*, uint16_t, MenuPanelView *);
uint32_t							editTemplateIP		 ( const __FlashStringHelper*, uint32_t, MenuPanelView *);
DateTime							editTemplateDateTime ( const __FlashStringHelper*, DateTime, MenuPanelView *);
uint32_t							editTemplateUInt32	 ( const __FlashStringHelper*, uint32_t, MenuPanelView *);
uint32_t							editTemplatePassword ( const __FlashStringHelper*, MenuPanelView *);
int									editTemplateMfrID	 ( byte&, MenuPanelView*);
#pragma endregion

#pragma region MenuOption Array Assignments
MenuOption innerMenuAcesso[] = {
	{ "Adicionar Cartao",			okMenuAcessoAdicionarCartao,			gotoRootMenu },
	{ "Revogar Cartao",				okMenuAcessoRevogarCartao,				gotoRootMenu },
	{ "Adicionar Mestre",			okMenuAcessoAdicionarMestre,			gotoRootMenu },
	{ "Revogar Mestre",				okMenuAcessoRevogarMestre,				gotoRootMenu },
	{ "Alterar Senha",				okMenuAcessoAlterarSenha,				gotoRootMenu }
};

MenuOption innerMenuDataHora[] = {
	{ "Ajustar Data",				okMenuDataHoraAjustarData,				gotoRootMenu }, 
	{ "Servidor NTP",				okMenuDataHoraServidorNTP,				gotoRootMenu }  
};														  
														  
MenuOption innerMenuRede[] = {							  
	{ "DHCP",						okMenuRedeDHCP,							gotoRootMenu }, 
	{ "Endereco IP",				okMenuRedeEnderecoIP,					gotoRootMenu }, 
	{ "Mascara",					okMenuRedeMascara,						gotoRootMenu }, 
	{ "Gateway",					okMenuRedeGateway,						gotoRootMenu }  
};

MenuOption innerMenuServidor[] = {
	{ "Endereco IP",				okMenuServidorEnderecoIP,				gotoRootMenu }, 
	{ "Porta",						okMenuServidorPorta,					gotoRootMenu }, 
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
	setActivatedMenuOption( innerMenuAcesso, 5 );
}
void gotoDataHora( MenuPanelView *mpv ) {
	setActivatedMenuOption( innerMenuDataHora, 3 );
}
void gotoRede( MenuPanelView *mpv ) {
	setActivatedMenuOption( innerMenuRede, 4 );
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
		
		mpv->accessRegWriter->Write( result );

		System::LED_GREEN_ON();
		mpv->lcd->print( F( "Acesso permitido" ) );
		mpv->lcd->setCursor( 0, 1 );
		
		for ( int i = 0; i < 4; i++ )
			mpv->lcd->print( id[i], HEX );
	}
	else {
		System::LED_RED_ON();
		mpv->lcd->println( F( "Nenhum cartao lido" ) );
	}

	delay( 2000 );
	System::LED_ALL_OFF();
}
void okMenuAcessoRevogarCartao( MenuPanelView *mpv ) {

}
void okMenuAcessoAdicionarMestre( MenuPanelView *mpv ) {
	byte id[4];

	int isCardRead = editTemplateMfrID( *id, mpv );
	mpv->lcd->clear();

	if (isCardRead > 0) {
		AccessReg result;

		for (int i = 0; i < 4; i++)
			result.mifareID[i] = id[i];

		result.isMaster = true;
		
		mpv->accessRegWriter->Write( result );

		mpv->lcd->print( F( "Acesso permitido" ) );
		mpv->lcd->setCursor( 0, 1 );
		
		for ( int i = 0; i < 4; i++ )
			mpv->lcd->print( id[i], HEX );
	}
	else {
		mpv->lcd->println( F( "Nenhum cartao lido" ) );
	}

	delay( 2000 );
}
void okMenuAcessoRevogarMestre( MenuPanelView *mpv ) {

}
void okMenuAcessoAlterarSenha( MenuPanelView *mpv ) {
	bool isActualPasswordCorrect = mpv->CheckPassword();
	mpv->lcd->clear();
	
	if ( !isActualPasswordCorrect ) {
		mpv->lcd->print( F ( "   Senha atual" ) );
		mpv->lcd->setCursor( 0, 1 );
		mpv->lcd->print( F ( "    incorreta" ) );

		System::NOTIFY_ERROR();
		return;
	}

	uint32_t newPassword = editTemplatePassword( F( "Nova senha:" ), mpv );
	if ( newPassword == INVALID_PASSWORD ) {
		mpv->lcd->print( F ( " Senha invalida" ) );

		System::NOTIFY_ERROR();
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
		return;
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
	DateTime dateTime = System::DT_getDateTime( mpv->rtc );
	DateTime newdateTime = editTemplateDateTime( F( "Data e hora:" ), dateTime, mpv );
	
	System::DT_setDateTime( newdateTime, mpv->rtc );
}
void okMenuDataHoraServidorNTP( MenuPanelView *mpv ) {
	uint32_t ip = System::DT_getNTPIpAddress();
	uint32_t newIp = editTemplateIP( F( "IP Servidor NTP:" ), ip, mpv );
	
	System::DT_setNTPIpAddress( newIp );
}
#pragma endregion

#pragma region Network Menu
void okMenuRedeDHCP( MenuPanelView *mpv ) {
	bool dhcpFlag = System::NW_getIsDHCP();
	String sSim = "Sim";
	String sNao = "Nao";

	bool newDhcpFlag = editTemplateFlag( F( "DHCP:" ), sSim, sNao, dhcpFlag, mpv );

	System::NW_setIsDHCP(newDhcpFlag);
}
void okMenuRedeEnderecoIP( MenuPanelView *mpv ) {
	uint32_t ip = System::NW_getIpAddress();
	uint32_t newIp = editTemplateIP( F( "IP de Rede:" ), ip, mpv );

	System::NW_setIpAddress( newIp );
}
void okMenuRedeMascara( MenuPanelView *mpv ) {
	uint32_t mask = System::NW_getMask();
	uint32_t newMask = editTemplateIP( F( "Mascara de Rede:" ), mask, mpv );
	
	System::NW_setMask( newMask );
}
void okMenuRedeGateway( MenuPanelView *mpv ) {
	uint32_t gateway = System::NW_getGateway();
	uint32_t newGateway = editTemplateIP( F( "Mascara de Rede:" ), gateway, mpv );
	
	System::NW_setGateway( newGateway );
}
#pragma endregion

#pragma region ServerMenu
void okMenuServidorEnderecoIP( MenuPanelView *mpv ) {
	uint32_t ip = System::SRV_getIpAddress();
	uint32_t newIp = editTemplateIP( F( "IP do Servidor:" ), ip, mpv );

	System::SRV_setIpAddress( newIp );
}
void okMenuServidorPorta( MenuPanelView *mpv ) {
	uint16_t port = System::SRV_getPort();
	uint16_t newPort = editTemplateUInt16( F( "Porta Servidor:" ), port, mpv );
	
	System::SRV_setPort( newPort );
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

boolean editTemplateFlag( const __FlashStringHelper * editorDisplay,
						  String& optFlagOn,
						  String& optFlagOff,
						  boolean current,
						  MenuPanelView *mpv ) {
	const __FlashStringHelper * bktsUnchecked = F( "[ ]" );
	const __FlashStringHelper * bktsChecked = F(" [*]" );

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
	
		mpv->lcd->print( result? bktsChecked : bktsUnchecked );
		mpv->lcd->print( optFlagOn );
	
		mpv->lcd->print( ' ' );

		mpv->lcd->print( result? bktsUnchecked : bktsChecked );
		mpv->lcd->print( optFlagOff );

		c = mpv->keyPadListener->WaitForInput();
	} while ( c != '#' && c != '*' );

	if ( c == '#' )
		return current;

	return result;	
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

uint32_t editTemplateIP( const __FlashStringHelper * editorDisplay, uint32_t current, MenuPanelView *mpv ) {
	uint32_t ip[4] = {
		( current >> 24 ),
		( current & 0x00FF0000 ) >> 16,
		( current & 0x0000FF00 ) >> 8,
		( current & 0x000000FF )
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
					return current;
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

	return ( uint32_t )( ( ip[0] << 24 ) | ( ip[1] << 16 ) | ( ip[2] << 8 ) | ip[3] );
}

DateTime editTemplateDateTime( const __FlashStringHelper * editorDisplay, DateTime current, MenuPanelView *mpv )
{
	uint8_t values[] = { 
		current.hour(),
		current.minute(),
		current.day(),
		current.month(),
		current.year() % 100
	};
	
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
							  IAccessRegWriter* accessRegWriter )
	: ViewBase( lcd ) {
	this->hasNewFrame = isInMenu = false;
	this->keyPadListener = keyPad;
	this->rtc = rtc;
	this->idProvider = idProvider;
	this->accessRegWriter = accessRegWriter;
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

MenuPanelView::~MenuPanelView() { }