#ifndef SPECIFICEVENTFUNCTOR_H
#define SPECIFICEVENTFUNCTOR_H

//Todas as implementações estão no .h porque o AVR não é compatível com templates no .cpp

#include "EventFunctor.h"

template <class TClass, class TEventArgs> class SpecificEventFunctor : public EventFunctor<TEventArgs>
{
private:
	void (TClass::*fpt)(const TEventArgs*);   // pointer to member function
	TClass* pt2Object;                  // pointer to object

public:

	// constructor - takes pointer to an object and pointer to a member and stores
	// them in two private variables
	SpecificEventFunctor(TClass* _pt2Object, void(TClass::*_fpt)(const TEventArgs*))
	{
		pt2Object = _pt2Object;  fpt = _fpt;
	};

	// override operator "()"
	virtual void operator()(const TEventArgs* e)
	{
		(*pt2Object.*fpt)(e);
	};              // execute member function

	// override function "Call"
	virtual void Call(const TEventArgs* e)
	{
		(*pt2Object.*fpt)(e);
	};
};

#endif /* SPECIFICEVENTFUNCTOR_H */