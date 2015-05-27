#ifndef TFUNCTOR_H
#define TFUNCTOR_H

//Todas as implementações estão no .h porque o AVR não é compatível com templates no .cpp

template <class TEventArgs> class EventFunctor
{
public:

	// two possible functions to call member function. virtual cause derived
	// classes will use a pointer to an object and a pointer to a member function
	// to make the function call
	virtual void operator()(const TEventArgs* e) = 0;  // call using operator
	virtual void Call(const TEventArgs* e) = 0;        // call using function
};

#endif /* TFUNCTOR_H */