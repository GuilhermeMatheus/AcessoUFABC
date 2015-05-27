#include "KeyPadLongPressListener.h"
#include "KeyDownEventArgs.h"
#include "SpecificEventFunctor.h"

KeyPadLongPressListener::KeyPadLongPressListener(char charToObserve, int thresholdInMilliseconds, KeyPadListener* listener)
{
	this->c = charToObserve;
	this->threshold = thresholdInMilliseconds;
	this->listener = listener;
	this->firstPress = 0;
	this->lastCycle = 0;

	this->listener->
		AddKeyDownEventHandler(new SpecificEventFunctor<KeyPadLongPressListener, KeyDownEventArgs>
									(this, &KeyPadLongPressListener::OnKeyDown));
}

void KeyPadLongPressListener::OnKeyDown(const KeyDownEventArgs* e)
{
	bool mustReset = false;
	unsigned long now = millis();

	if (e->Key == this->c && e->cycle == ++lastCycle)
	{
		pressedTime = now - firstPress;
		if (pressedTime >= threshold)
		{
			EventArgs* ea = new EventArgs();
		
			this->function->Call(ea);
			mustReset = true;

			delete(ea);
		}
	}
	else
	{
		mustReset = true;
	}

	if (mustReset)
	{
		firstPress = now;
		lastCycle = e->cycle;
	}
}

void KeyPadLongPressListener::AddEventHandler(EventFunctor<EventArgs>* function)
{
	this->function = function;
}


KeyPadLongPressListener::~KeyPadLongPressListener()
{
}
