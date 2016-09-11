////////////////////////////////////////////////////////////////////////////////
// arudino wraper functions
// by dotnfc@163.com
// 2016/09/10 18:16:00

#include "Arduino.h"


/*==============================================================================
 * \brief arduino like 'tone ()'
 */
void tone(PwmOut &buz, long frequency)
{
    if (frequency == 0) {
        buz = 0;
        return;
    }
    
    float period;

    // calculate the period of the note in turn play
    period = 1000000 / frequency;
    // set the period calculated at the PWM output
    buz.period_us (period);
    // establish a duty cycle of 50%
    buz.write (0.50f);  
}
 

