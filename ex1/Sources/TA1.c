#include <msp430.h>
#include "..\base.h"
#include "TA1.h"
#include "event.h"

#define BTSIZE 2
#define CNTMAX 5

typedef enum {s0, s1} TState;

typedef struct {
    UChar cnt;
    TState state;
} var;

LOCAL var vars[BTSIZE];
LOCAL UChar currBT;

LOCAL const struct {
    volatile UChar * const port;
    const UChar mask;
    const TEvent msg;
} btns[BTSIZE] = {
          {(UChar *)(&P1IN), BIT0, EVENT_BTN1},
          {(UChar *)(&P1IN), BIT1, EVENT_BTN2}
};

#pragma FUNC_ALWAYS_INLINE(TA1_init)
GLOBAL Void TA1_init(Void) {
   vars[0].cnt = 0;
   vars[0].state = s0;

   vars[1].cnt = 0;
   vars[1].state = s0;

   currBT = 0;

   TA1CTL   = 0; // stop mode, disable and clear flags
   TA1CCTL0 = C; // no capture mode, compare mode
                 // clear and disable interrupt flag

   // Fuer ein 5ms entprellen:
   //  => Standard ACLK BOARDFREQ: 32.768 kHz => ACLK_BRD_FREQ * 5 ms = 164 => 164 -1 = 163
   TA1CCR0  = 0xA3;             // set up Compare Register
   TA1EX0   = TAIDEX_0;        // set up expansion register /1
   TA1CTL   = TASSEL__ACLK     // 613.75 kHz
            | MC__UP           // Up Mode
            | ID__1            // input divider /1
            | TACLR            // clear and start Timer
            | TAIE;            // enable interrupt
}


#pragma vector = TIMER1_A1_VECTOR
__interrupt Void TIMER1_A1_ISR(Void) {

   /*
    * Der Inhalt der ISR ist zu implementieren
    */
    // NUR einen BT zur selben Zeit abfragen
    var *v = &vars[currBT];

    if (!TSTBIT(*btns[currBT].port, btns[currBT].mask)) {
        if (++(v->cnt) GE CNTMAX) {
            v->cnt = CNTMAX;
            if (v->state EQ s0) {
                v->state = s1;
                Event_set(btns[currBT].msg);
                __low_power_mode_off_on_exit();
            }
        }
    } else {
        if (--(v->cnt) LE 0) {
            v->cnt = 0;
            v->state = s0;
        }
    }
    // Wechsel der Tasten
    currBT ^= 1;

    CLRBIT(TA1CTL, TAIFG);
}
