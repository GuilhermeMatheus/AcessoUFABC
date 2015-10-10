#include "DateTimeAdapter.h"

DateTime DateTimeAdapter::ToUTC( DateTime value ) {
	int hour = value.hour(),
		day = value.day(),
		month = value.month(),
		year = value.year(),
		dow = value.dayOfWeek() - 1;

	int daylightSavingOffset = IsDaylightSaving(day, month, year, dow) ? -1 : 0;
	int timeZoneOffset = System::DT_getTimeZoneOffset();

	int offset = timeZoneOffset + daylightSavingOffset;

	AddHours( &hour, &day, &month, &year, -offset );

	return DateTime(year, month, day, hour, value.minute(), value.second());
}

DateTime DateTimeAdapter::ToLocalTime( DateTime value ) {
	int hour = value.hour(),
		day = value.day(),
		month = value.month(),
		year = value.year(),
		dow = value.dayOfWeek() - 1;

	int daylightSavingOffset = IsDaylightSaving(day, month, year, dow) ? -1 : 0;
	int timeZoneOffset = System::DT_getTimeZoneOffset();

	int offset = timeZoneOffset + daylightSavingOffset;

	SubtractHours( &hour, &day, &month, &year, -offset );

	return DateTime(year, month, day, hour, value.minute(), value.second());
}

int DateTimeAdapter::ToString( DateTime value, String &target ) {
	const char _barra = '/';
	const char _zero = '0';

	target = "";

	if (value.hour() < 10)
		target.concat(_zero);
	target.concat(value.hour());
	target.concat(':');

	if (value.minute() < 10)
		target.concat(_zero);
	target.concat(value.minute());
	target.concat(' ');

	if (value.day() < 10)
		target.concat(_zero);
	target.concat(value.day());
	target.concat(_barra);

	if (value.month() < 10)
		target.concat(_zero);
	target.concat(value.month());
	target.concat(_barra);

	target.concat(value.year() - 2000);

	return 1;
}

#pragma region InternalMethods
void DateTimeAdapter::AddHours(int *hour, int *day, int *month, int *year, int hours) {
	int sumHours = (*hour + hours);

	if ( sumHours < 24) {
		*hour = sumHours;
	}
	else {
		int days = sumHours / 24;
		*hour  = sumHours % 24;

		AddDays( day, month, year, days);
	}
}

void DateTimeAdapter::AddDays( int *day, int *month, int *year, int days ) {
	int monthDays = MonthDays(*month, *year);
	int sumDays = *day + days;

	if (sumDays <= monthDays) {
		*day = sumDays;
	}
	else {
		int months = sumDays / monthDays;
		*day  = sumDays % monthDays;

		AddMonths( month, year, months);
	}
}

void DateTimeAdapter::AddMonths( int *month, int *year, int months ) {
	int sumMonths = *month + months;
	
	if (sumMonths <= 12) {
		*month = sumMonths;
	}
	else {
		*month = sumMonths % 12;
		*year = *year + sumMonths / 12;
	}
}

// Algoritmo de Meeus/Jones/Butcher
void DateTimeAdapter::CalcEasterDay(int *day, int *month, int year) {
	int a = year % 19;
	int b = year / 100;
	int c = year % 100;
	int d = b / 4;
	int e = b % 4;
	int f = (b + 8) / 25;
	int g = (b - f + 1) / 3;
	int h = (19 * a + b - d - g + 15) % 30;
	int i = c / 4;
	int k = c % 4;
	int l = (32 + 2 * e + 2 * i - h - k) % 7;
	int m = (a + 11 * h + 22 * l) / 451;
	
	*month = (h + l - 7 * m + 114) / 31;
	*day = ((h + l - 7 * m + 114) % 31) + 1;
}

int DateTimeAdapter::MonthDays(int month, int year) {
	switch (month) {
	case 1: case 3: case 5: case 7: case 8: case 10: case 12:
		return 31;
	case 2:
		if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)))
			return 29;
		else
			return 28;
	case 4: case 6: case 9: case 11:
		return 30;
	default:
		return -1;
	}
}

// congru�ncia de Zeller
int DateTimeAdapter::DayOfTheWeek(int day, int month, int year) {
	if (month < 3) {
		month += 12;
		--year;
	}
	int k = year % 100;
	int j = year / 100;
	return (day + 13 * (month + 1) / 5 + k + k / 4 + j / 100 - 2 * j + 4) % 7;
}

void DateTimeAdapter::SubtractHours(int *hour, int *day, int *month, int *year, int hours) {
	if (*hour > hours) {
		*hour = *hour- hours;
	}
	else {
		*hour = 24 - (hours - *hour);
		SubtractDays(day, month, year, 1);
	}
}

void DateTimeAdapter::SubtractDays( int *day, int *month, int *year, int days ) {
	while (days > 0) {
		if (*day > days) {
			*day = *day - days;
			days = 0;
		}
		else {
			days = days - *day;
			if (*month == 1) {
				--(*year);
				*month = 12;
				*day = 31;
			}
			else {
				--(*month);
				*day = MonthDays(*month, *year);
			}
		}
	}
}

void DateTimeAdapter::CalcCarnivalSunday(int *day, int *month, int year) {
	CalcEasterDay(day, month, year);
	SubtractDays(day, month, &year, 49);
}

bool DateTimeAdapter::IsCarnivalSunday( int day, int month, int year ) {
	int carnivalDay, carnivalMonth, carnivalYear = year;
	CalcCarnivalSunday(&carnivalDay, &carnivalMonth, carnivalYear);

	return (day == carnivalDay && month == carnivalMonth && year == carnivalYear);
}

int DateTimeAdapter::ThirdSunday( int month, int year ) {
	int firstSunday = 1;
	int dow = DayOfTheWeek(firstSunday, month, year);
	if (dow != 0) {
		firstSunday += (7 - dow);
	}
	return firstSunday + 14;
}

bool DateTimeAdapter::IsDaylightSaving( int day, int month, int year, int dow ) {

	uint8_t dlsMode = System::DT_getAutoDaylightSaving();
	
	if (dlsMode == 1) 
		return true;  //In daylightSaving, by manual configuration
	
	if (dlsMode == 2) 
		return false; //Not in daylightSaving, by manual configuration

	if (dow == -1)
		dow = DayOfTheWeek(day, month, year);

	if (month == 11 || month == 12 || month == 1)
		return true;

	if (month == 10) {
		if (day < 15) return false;
		if (day >= 21) return true;
		// vai para o domingo anterior
		if (day - dow >= 15 && day - dow < 21)
			return true;
		else
			return false;
	}
	if (month == 2) {
		if (day < 15) return true;
		int endDay = ThirdSunday(month, year);
		if (IsCarnivalSunday(endDay, month, year)) {
			endDay += 7;
		}
		if (day >= endDay)
			return false;
		else
			return true;
	}
	return false;
}
#pragma endregion
