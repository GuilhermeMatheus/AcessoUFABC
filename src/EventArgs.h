#ifndef EVENTARGS_H
#define EVENTARGS_H

/**
* Fornece dados para eventos que podem ser manipulados completamente em um manipulador de eventos
*/
class EventArgs
{
public:
	EventArgs() { Handled = false; }
	~EventArgs() { }

	/**
	* Obt�m ou define um valor que indica se o manipulador de eventos tratou completamente o evento ou se o sistema deve continuar o seu pr�prio processamento.
	*/
	bool Handled;
};

#endif /* EVENTARGS_H */