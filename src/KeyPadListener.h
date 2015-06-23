#ifndef KEYPADLISTENER_H
#define KEYPADLISTENER_H

#define ROW_1 3
#define ROW_2 2
#define ROW_3 1
#define ROW_4 0

#define THRESHOLD_TOLERANCE 30

#define MIN_THRESHOLD_ROW_1 128
#define MAX_THRESHOLD_ROW_1 384 - THRESHOLD_TOLERANCE

#define MIN_THRESHOLD_ROW_2 384 + THRESHOLD_TOLERANCE
#define MAX_THRESHOLD_ROW_2 640 - THRESHOLD_TOLERANCE

#define MIN_THRESHOLD_ROW_3 640 + THRESHOLD_TOLERANCE
#define MAX_THRESHOLD_ROW_3 896 - THRESHOLD_TOLERANCE

#define MIN_THRESHOLD_ROW_4 896 + THRESHOLD_TOLERANCE

#include <Arduino.h>

#include "System.h"

/**
* Classe responsável por verificar o estado de um teclado matricial numérico.
*/
class KeyPadListener
{
private:
	int8_t pinColumnOne, pinColumnTwo, pinColumnThree, actualFunctionIndex;
	uint32_t cycle;
	
protected:
	char keyTable[3][4];
	int GetRowFromColumn(int8_t column);

public:
	/**
	* Cria uma instância de KeyPadListener.
	*
	* @param pinColumnOne Número do pino analógico responsável pela primeira coluna da matriz, com os dígitos [1], [4], [7] e [*]
	* @param pinColumnTwo Número do pino analógico responsável pela segunda coluna da matriz, com os dígitos [2], [5], [8] e [0]
	* @param pinColumnThree Número do pino analógico responsável pela terceira coluna da matriz, com os dígitos [3], [6], [9] e [#]
	*/
	KeyPadListener(int8_t pinColumnOne, int8_t pinColumnTwo, int8_t pinColumnThree);
	~KeyPadListener();

	/**
	* Verifica o estado do teclado.
	* 
	* @return Retorna o dígito pressionado ou -1, caso nenhuma tecla esteja acionada
	*/
	int16_t GetState();
	
	/**
	* Para a aplicação até que uma tecla seja pressionada.
	* 
	* @return Retorna o dígito pressionado
	*/
	char WaitForInput();
	/**
	* Para a aplicação até que uma tecla de dígito seja pressionada.
	* 
	* @return Retorna o número do dígito pressionado
	*/
	uint8_t WaitForDigitInput();
	/**
	* Verifica por quanto tempo uma tecla é pressionada
	* 
	* @param max Tempo em millisegundos máximo a ser contado.
	* @param c Tecla a ser assistida.
	* @return Retorna o tempo em que a tecla foi pressionada ou {@code max} quando o tempo limite for excedido
	*/
	long GetPressTime(char c, long max);

};

#endif /* KEYPADLISTENER_H */