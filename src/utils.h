#define DEBUG

#ifdef DEBUG
#define _LOG(msg) Serial.println(F(msg))
#define _LOGS(msg) Serial.println(msg)
#else
#define _LOG(msg) ;
#define _LOGS(msg) ;
#endif /* DEBUG */
