#ifndef MENUPANELVIEW_H
#define MENUPANELVIEW_H

#define KEY_UP			'2'
#define KEY_DOWN		'8'
#define KEY_LEFT		'4'
#define KEY_RIGHT		'6'
#define KEY_OK			'*'
#define KEY_CANCEL		'#'

/**
* Em métodos que exigem a digitação de senha do usuário, esta macro representa uma senha inválida.
*/
#define INVALID_PASSWORD	0

#include <Arduino.h>

#include "ViewBase.h"
#include "KeyPadListener.h"
#include "IAccessRegWriter.h"
#include "NTPDateTimeProvider.h"
#include "IIDProvider.h"
#include "RTClib.h"
#include "DateTimeAdapter.h"

/**
* Classe responsável por exibir e interfacear as configurações do sistema com o usuário.
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
	* Cria uma instância de MenuPanelView.
	* 
	* @param lcd Lcd de destino a exibir as configurações de menu.
	* @param keyPadListener Teclado a ser escutado para a digitação de valores do usuário.
	* @param rtc Controlador RTC a ser usado para configurações de data e hora.
	* @param idProvider Leitor de cartões a ser usado para as configurações relativas a inserção ou exclusão de acessos.
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
	* Verifica se a View está ativada e inicia o Loop contínuo da lógica da controladora até que o usuário saia do menu.
	*/
	void							Loop();
	
	/**
	* Ativa o menu. Deve ser chamado antes de Loop().
	*/
	void							Active();
	
	/**
	* Aguarda o usuário digitar a senha de acesso e a compara com o valor retornado por System::ACS_GetPassword()
	* 
	* @return Retorna True quando a senha for correta ou, caso contrário, False.
	*/
	bool							CheckPassword();

	/**
	* Verifica em System::GetIsSetup() se o terminal já foi configurado pela primeira vez e executa o Setup
	* para o usuário configurar as propriedades básicas: 
	*		-Senha;
	*		-Data e Hora;
	*		-Número do Terminal;
	*		-IP de servidor.
	* Após isto, invoca  System::ClearRegs() para limpar qualquer sujeira que possa vir na memória.
	*/
	void							RunSetupIfNeeded();

	~MenuPanelView();
};

#endif /* MENUPANELVIEW_H */