
#ifndef __ARDUINO_H__
#define __ARDUINO_H__

#include "mbed.h"
#include "HardwareSerial.h"


/**
 * \brief Returns the number of milliseconds since the Arduino board began running the current program.
 *
 * This number will overflow (go back to zero), after approximately 50 days.
 *
 * \return Number of milliseconds since the program started (uint32_t)
 */
//uint32_t millis( void );


/**
 * \brief Pauses the program for the amount of time (in miliseconds) specified as parameter.
 * (There are 1000 milliseconds in a second.)
 *
 * \param dwMs the number of milliseconds to pause (uint32_t)
 */
//inline void delay( uint32_t dwMs ) ;
#define delay wait_ms


/*==============================================================================
 * \brief arduino like 'tone ()'
 */
void tone(PwmOut &buz, long frequency);

#endif // __ARDUINO_H__
