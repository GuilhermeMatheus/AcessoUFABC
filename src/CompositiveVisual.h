#ifndef COMPOSITIVEVISUAL_H
#define COMPOSITIVEVISUAL_H

#include "IVisual.h"
class CompositiveVisual :
	public IVisual
{
private:
	int count;

public:
	CompositiveVisual();

	void Add(IVisual& visualElement);

	void Begin();
	void Clear();

	void ShowSuccess(const char message[]);
	void ShowWarning(const char message[]);
	void ShowError(const char message[]);
};

#endif /* COMPOSITIVEVISUAL_H */