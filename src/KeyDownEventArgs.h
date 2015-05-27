#ifndef KEYDOWNEVENTARGS_H
#define KEYDOWNEVENTARGS_H

#include "EventArgs.h"

class KeyDownEventArgs :
	public EventArgs
{
public:
	char Key;
	int cycle;

	KeyDownEventArgs(char key, int cycle)
	{
		this->Key = key;
		this->cycle = cycle;
	}
	~KeyDownEventArgs() { }
};

#endif /* KEYDOWNEVENTARGS_H */