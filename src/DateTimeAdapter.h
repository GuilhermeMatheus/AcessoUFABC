#ifndef DATETIMEADAPTER_H
#define DATETIMEADAPTER_H

#include <Arduino.h>
#include <RTClib.h>

#include "System.h"

class DateTimeAdapter
{
private:
	//dow = dia da semana: 0 (dom), 1 (seg), ..., 6 (sáb)
	static bool					IsDaylightSaving( int day, int month, int year, int dow = -1 );


	static void AddHours( int *hour, int *day, int *month, int *year, int hours );
	static void AddDays( int *day, int *month, int *year, int days );
	static void AddMonths( int *month, int *year, int months );

	static void	SubtractHours( int *hour, int *day, int *month, int *year, int hours );
	static void	SubtractDays( int *day, int *month, int *year, int days );
	static void SubtractMonths( int *month, int *year, int months );

	static void					CalcEasterDay( int *day, int *month, int year );
	static void					CalcCarnivalSunday( int *day, int *month, int year );
	static bool					IsCarnivalSunday( int day, int month, int year );
	static int					MonthDays( int day, int month );
	static int					ThirdSunday( int month, int year );
	static int					DayOfTheWeek( int day, int month, int year );

public:
	/*
	 * Converte um DateTime no formato local (fuso horário + horário de verão) para o formato UTC
	 * 
	 * @param value Data base para conversão.
	 * @return Retorna a data {@code value} no formato UTC.
	*/
	static DateTime				ToUTC( DateTime value );
	
	/*
	 * Converte um DateTime no formato UTC para o formato local (levando em conta o fuso horário e horário de verão)
	 * 
	 * @param value Data base para conversão.
	 * @return Retorna a data {@code value} no formato local.
	*/
	static DateTime				ToLocalTime( DateTime value );
	
	/*
	 * Converte um DateTime para uma String no formato "HH/mm dd/MM/yyyy"
	 * 
	 * @param value Data base para conversão.
	 * @param target Destino do valor convertido.
	 * @return Retorna 1 caso a conversão tenha sido feita com sucesso ou, caso contrário, -1.
	*/
	static int					ToString( DateTime value, String &target );
};

#endif /* DATETIMEADAPTER_H */