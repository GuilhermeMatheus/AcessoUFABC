#include "CompositiveVisual.h"

IVisual *elementos[10];

CompositiveVisual::CompositiveVisual()
{
	count = 0;
}

void CompositiveVisual::Begin()
{
	for (int i = 0; i < count; i++)
		elementos[i]->Begin();
}
void CompositiveVisual::Clear()
{
	for (int i = 0; i < count; i++)
		elementos[i]->Clear();
}

void CompositiveVisual::Add(IVisual& visualElement)
{
	elementos[count++] = &visualElement;
}

void CompositiveVisual::ShowSuccess(const char message[])
{
	for (int i = 0; i < count; i++)
		elementos[i]->ShowSuccess(message);
}

void CompositiveVisual::ShowWarning(const char message[])
{
	for (int i = 0; i < count; i++)
		elementos[i]->ShowWarning(message);
}

void CompositiveVisual::ShowError(const char message[])
{
	for (int i = 0; i < count; i++)
		elementos[i]->ShowError(message);
}