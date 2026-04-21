#include <msp430.h>
#include "..\base.h"
#include "TA1.h"
#include "event.h"


#define CNTMAX 5

typedef enum {s0, s1} TState;

LOCAL struct {
    Int cnt;
    TState state;
} var1, var2;

LOCAL const struct {
    const UChar * const port;
    const UChar mask;
    const TEvent msg;
} btns[2] = {
          {(UChar *)(&P1IN), BIT0, EVENT_BTN1},
          {(UChar *)(&P2IN), BIT0, EVENT_BTN2}
};

#pragma FUNC_ALWAYS_INLINE(TA1_init)
GLOBAL Void TA1_init(Void) {
   var1.cnt = 0;
   SETBIT(var1.state, s0);

   var2.cnt = 0;
   SETBIT(var2.state, s0);
   TA1CTL   = 0; // stop mode, disable and clear flags
   TA1CCTL0 = C; // no capture mode, compare mode
                 // clear and disable interrupt flag
   TA1CCR0  = 0xFFFF;          // set up Compare Register
   TA1EX0   = TAIDEX_0;        // set up expansion register
   TA1CTL   = TASSEL__ACLK     // 613.75 kHz
            | MC__UP           // Up Mode
            | ID__1            // input divider
            | TACLR            // clear and start Timer
            | TAIE;            // enable interrupt
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt Void TIMER1_A1_ISR(Void) {

   /*
    * Der Inhalt der ISR ist zu implementieren
    */
    if (TSTBIT(*btns[0].port, btns[0].mask)) {
        if (--var1.cnt LT 0) {
            var1.cnt = 0;
            SETBIT(var1.state, s0);
        }
    }

    if (++var1.cnt GT CNTMAX-1) {
        var1.cnt = CNTMAX-1;
        if (var1.state EQ s0) {
            var1.state = s1;
            Event_set(btns[0].msg);
            __low_power_mode_off_on_exit();
        }
    }

    if (TSTBIT(*btns[1].port, btns[1].mask)) {
            if (--var2.cnt LT 0) {
                var2.cnt = 0;
                SETBIT(var2.state, s0);
            }
        }

     if (++var2.cnt GT CNTMAX-1) {
         var2.cnt = CNTMAX-1;
         if (var2.state EQ s0) {
             var2.state = s1;
             Event_set(btns[1].msg);
             __low_power_mode_off_on_exit();
         }
     }
     CLRBIT(TA1CTL, TAIFG);
}
