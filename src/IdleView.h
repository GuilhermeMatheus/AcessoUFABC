#ifndef IDLEVIEW_H
#define IDLEVIEW_H

#include <Arduino.h>

#include "ViewBase.h"
#include "IDateTimeProvider.h"
#include "DateTimeAdapter.h"

/**
* Classe responsável por exibir a data e hora no display da controladora enquanto estiver em IDLE.
*/
class IdleView :
	public ViewBase
{
private:
	IDateTimeProvider					*DateProvider;
	bool								viewChanged;

protected:
	void								OnDraw();
	bool								HasNewFrame();

public:
	/**
	* Cria uma instância de IdleView.
	* 
	* @param lcd Lcd de destino a exibir as configurações de menu.
	* @param DateProvider Objeto a ser consultado para se obter a data e hora.
	*/ 
	IdleView( LiquidCrystal_I2C *lcd,
			  IDateTimeProvider *DateProvider );

	void								ViewChanged();

	~IdleView();
};

#endif /* IDLEVIEW_H */