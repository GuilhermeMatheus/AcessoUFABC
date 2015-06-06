#ifndef GUARDIANKEEPER_H
#define GUARDIANKEEPER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "AccessProvider.h"
#include "System.h"

class GuardianKeeper {
private:
	AccessProvider				*accessProvider;
	LiquidCrystal_I2C			*lcd;
	inline int					RestrictAccess( AccessAttemptResult &result );
	inline int					GiveAccess( AccessAttemptResult &result );

public:
	GuardianKeeper ( AccessProvider *accessProvider, LiquidCrystal_I2C *lcd );
	
	int							AllowAccess(byte &cardId);

	~GuardianKeeper();
};

#endif /* GUARDIANKEEPER_H */