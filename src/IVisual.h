#ifndef IVISUAL_H
#define IVISUAL_H

#include "Arduino.h"

class IVisual
{
public:
	/**
	* Inicia os recursos necessários para o funcionamento do objeto.
	*/
	virtual void Begin() = 0;

	/**
	* Limpa qualquer notificação para o usuário.
	*/
	virtual void Clear() = 0;

	/**
	* Notifica visualmente o usuário sobre uma ação que obteve Sucesso.
	* @param message  mensagem de sucesso.
	*/
	virtual void ShowSuccess(const char message[]) = 0;

	/**
	* Notifica visualmente o usuário sobre uma ação que necessita atenção.
	* @param message  mensagem de aviso.
	*/
	virtual void ShowWarning(const char message[]) = 0;
	
	/**
	* Notifica visualmente o usuário sobre uma ação que obteve Erro.
	* @param message  mensagem de erro.
	*/
	virtual void ShowError(const char message[]) = 0;

	virtual ~IVisual() {}
};

#endif /* IVISUAL_H */

