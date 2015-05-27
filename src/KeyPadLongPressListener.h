#ifndef KEYPADLONGPRESSLISTENER_H
#define KEYPADLONGPRESSLISTENER_H

#include "KeyPadListener.h"
#include "EventFunctor.h"
#include "EventArgs.h"

class KeyPadLongPressListener
{
private:
	int lastCycle;
	unsigned long firstPress;
	unsigned long pressedTime;
	char c;
	int threshold;
	KeyPadListener* listener;
	EventFunctor<EventArgs>* function;

public:
	KeyPadLongPressListener(char charToObserve, int thresholdInMilliseconds, KeyPadListener* listener);
	~KeyPadLongPressListener();

	void OnKeyDown(const KeyDownEventArgs* e);
	void AddEventHandler(EventFunctor<EventArgs>* function);
};

#endif /* KEYPADLONGPRESSLISTENER_H */