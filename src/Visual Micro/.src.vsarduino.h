#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
//Board = Arduino Uno
#define __AVR_ATmega328P__
#define 
#define ARDUINO 101
#define ARDUINO_MAIN
#define __AVR__
#define F_CPU 16000000L
#define __cplusplus
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define __attribute__(x)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int
            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}

//
void initConfigurationsMenu();
String _converter(byte& value);
void debugLoop();
//

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\AcessoUFABC.ino"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\AccessProvider.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\AccessReg.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\BuzzerVisual.cpp"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\BuzzerVisual.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\CompositiveVisual.cpp"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\CompositiveVisual.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\EepromAccessRegWriter.cpp"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\EepromAccessRegWriter.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\EventArgs.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\EventFunctor.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\IAccessRegWriter.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\IDateTimeProvider.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\IIDProvider.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\IMifareIDProvider.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\IVisual.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\IdleView.cpp"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\IdleView.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\KeyDownEventArgs.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\KeyPadListener.cpp"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\KeyPadListener.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\KeyPadLongPressListener.cpp"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\KeyPadLongPressListener.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\LedsVisual.cpp"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\LedsVisual.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\MenuPanelView.cpp"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\MenuPanelView.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\MifareIDProvider.cpp"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\MifareIDProvider.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\NetworkAccessProvider.cpp"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\NetworkAccessProvider.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\RTCDateTimeProvider.cpp"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\RTCDateTimeProvider.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\SpecificEventFunctor.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\System.cpp"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\System.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\ViewBase.h"
#include "C:\Users\gcosta\Documents\GitHub\AcessoUFABC\src\utils.h"
#endif
