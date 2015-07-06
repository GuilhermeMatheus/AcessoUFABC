#ifndef MENUPANELVIEW_H
#define MENUPANELVIEW_H

#define KEY_UP			'2'
#define KEY_DOWN		'8'
#define KEY_LEFT		'4'
#define KEY_RIGHT		'6'
#define KEY_OK			'*'
#define KEY_CANCEL		'#'

/**
* Em m�todos que exigem a digita��o de senha do usu�rio, esta macro representa uma senha inv�lida.
*/
#define INVALID_PASSWORD	0

#include <Arduino.h>

#include "ViewBase.h"
#include "KeyPadListener.h"
#include "IAccessRegWriter.h"
#include "NTPDateTimeProvider.h"
#include "IIDProvider.h"
#include "RTClib.h"

/**
* Classe respons�vel por exibir e interfacear as configura��es do sistema com o usu�rio.
*/
class MenuPanelView :
	public ViewBase {
private:
	bool							hasNewFrame;

protected:
	bool							TryToScroll( int8_t delta );
	void							ListenKeypad();
	void							OnDraw();
	bool							HasNewFrame();

public:
	KeyPadListener					*keyPadListener;
	RTC_DS1307						*rtc;
	IIDProvider						*idProvider;
	IAccessRegWriter				*accessRegWriter;
	NTPDateTimeProvider				*ntpDateTimeProvider;
	
	/**
	* Cria uma inst�ncia de MenuPanelView.
	* 
	* @param lcd Lcd de destino a exibir as configura��es de menu.
	* @param keyPadListener Teclado a ser escutado para a digita��o de valores do usu�rio.
	* @param rtc Controlador RTC a ser usado para configura��es de data e hora.
	* @param idProvider Leitor de cart�es a ser usado para as configura��es relativas a inser��o ou exclus�o de acessos.
	* @param accessRegWriter Objeto a ser usado para salvar ou excluir registros de acesso.
	* @param ntpDateTimeProvider Provider de data e hora por servidor NTP.
	*/
	MenuPanelView( LiquidCrystal_I2C   *lcd, 
				   KeyPadListener	   *keyPadListener,
				   RTC_DS1307		   *rtc,
				   IIDProvider		   *idProvider,
				   IAccessRegWriter    *accessRegWriter,
				   NTPDateTimeProvider *ntpDateTimeProvider );
	
	/**
	* Verifica se a View est� ativada e inicia o Loop cont�nuo da l�gica da controladora at� que o usu�rio saia do menu.
	*/
	void							Loop();
	
	/**
	* Ativa o menu. Deve ser chamado antes de Loop().
	*/
	void							Active();
	
	/**
	* Aguarda o usu�rio digitar a senha de acesso e a compara com o valor retornado por System::ACS_GetPassword()
	* 
	* @return Retorna True quando a senha for correta ou, caso contr�rio, False.
	*/
	bool							CheckPassword();

	~MenuPanelView();
};

#endif /* MENUPANELVIEW_H */