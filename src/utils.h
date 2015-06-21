#ifndef DEBUG
	#define DEBUG
#endif /* DEBUG */

#ifdef DEBUG
	#define _LOG( msg )  Serial.println(F(msg))
	#define _LOGS( msg ) Serial.println(msg)
	#define _LOGN( msg, base ) Serial.println(msg, base)
#else
	#define _LOG( msg );
	#define _LOGS( msg );
	#define _LOGN( msg, base ) ;
#endif /* DEBUG */