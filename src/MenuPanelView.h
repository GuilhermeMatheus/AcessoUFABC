#ifndef MENUPANELVIEW_H
#define MENUPANELVIEW_H

#define OPTIONS_COUNT	 1
#define KEY_UP			'2'
#define KEY_DOWN		'8'
#define KEY_LEFT		'4'
#define KEY_RIGHT		'6'
#define KEY_OK			'*'
#define KEY_CANCEL		'#'

#include <Arduino.h>

#include "ViewBase.h"
#include "KeyPadListener.h"
#include "IAccessRegWriter.h"
#include "IIDProvider.h"
#include "RTClib.h"

/**
* Classe responsável por exibir e interfacear as configurações do sistema com o usuário.
*/
class MenuPanelView :
	public ViewBase {
private:
	static const char				Up_Arrow    = ( char )  8;  // 0x0F10;
	static const char				Down_Arrow  = ( char )  9;  // 0x0C10;
	static const char				Left_Arrow  = ( char ) 18;  // 0x0E10;
	static const char				Right_Arrow = ( char ) 19;  // 0x0D10;
	
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
	
	/**
	* Cria uma instância de MenuPanelView.
	* 
	* @param lcd Lcd de destino a exibir as configurações de menu.
	* @param keyPadListener Teclado a ser escutado para a digitação de valores do usuário.
	* @param rtc Controlador RTC a ser usado para configurações de data e hora.
	* @param idProvider Leitor de cartões a ser usado para as configurações relativas a inserção ou exclusão de acessos.
	* @param accessRegWriter Objeto a ser usado para salvar ou excluir registros de acesso.
	*/
	MenuPanelView( LiquidCrystal_I2C  *lcd, 
				   KeyPadListener	  *keyPadListener,
				   RTC_DS1307		  *rtc,
				   IIDProvider		  *idProvider,
				   IAccessRegWriter   *accessRegWriter );
	
	/**
	* Verifica se a View está ativada e inicia o Loop contínuo da lógica da controladora até que o usuário saia do menu.
	*/
	void							Loop();
	
	/**
	* Ativa o menu. Deve ser chamado antes de Loop().
	*/
	void							Active();
	
	~MenuPanelView();
};

#endif /* MENUPANELVIEW_H */