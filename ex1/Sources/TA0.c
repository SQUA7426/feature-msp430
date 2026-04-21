#include <msp430.h>
#include "..\base.h"
#include "TA0.h"

/*
 * Man soll sich eine geeignete Datenstruktur �berlegen,
 * die eine laufzeiteffiziente Ausf�hrung der ISR erm�glicht.
 *
*/
#define HIGH    0x8000
#define LOW     0x0000

#define ACKFRQ  613.75 // kHz
#define TICK(t) ((UInt)(((ACKFRQ * t) / 4.0) / 5.0) - 1)
#define T_TICK 500

#define SIZE 36
#define TABSIZE 6
LOCAL const UInt* mA[SIZE] = {
    HIGH | TICK(T_TICK * 4),
    LOW  | TICK(T_TICK * 1),
    0,
    HIGH | TICK(T_TICK * 15e-1),
    LOW  | TICK(T_TICK * 15e-1),
    0,
    HIGH | TICK(T_TICK * 5e-1),
    LOW  | TICK(T_TICK * 5e-1),
    0,
    LOW  | TICK(T_TICK * 1),
    HIGH | TICK(T_TICK * 1),
    LOW  | TICK(T_TICK * 5),
    0,
    LOW  | TICK(T_TICK * 1),
    HIGH | TICK(T_TICK * 1),
    LOW  | TICK(T_TICK * 1),
    HIGH | TICK(T_TICK * 1),
    LOW  | TICK(T_TICK * 5),
    0,
    LOW  | TICK(T_TICK * 1),
    HIGH | TICK(T_TICK * 1),
    LOW  | TICK(T_TICK * 1),
    HIGH | TICK(T_TICK * 1),
    LOW  | TICK(T_TICK * 1),
    HIGH | TICK(T_TICK * 1),
    LOW  | TICK(T_TICK * 5),
    0
};

LOCAL const UInt* outerPtr;
LOCAL const UInt* ptr;

GLOBAL Void set_blink_muster(UInt arg) {
/*
 * Die Funktion muss so erweitert werden,
 * dass ein Blinkmuster selektiert wird.
 * Diese L�sung h�ngt stark von der gew�hlten
 * Datenstruktur ab.
 */
 // EVENT_BTN2 => modulo-Zaehler
    if (arg < TABSIZE -1) {
        outerPtr = mA[arg-1];
    } else {
        outerPtr = mA[0];
    }
    ptr = outerPtr;
}

/*
* times = [2500, 1500, 500, 2500, 3500, 4500] (ms)
* timespan = 2000 ms
* Timer_clock = = 613.75 kHz =ACKFRQ
* Teilungsfaktor = Timer_CLOCK * timespan = 1_227_500
*
* 16 Bit timer : 2**16 = 65_536
* Skalierungsfaktor = Teilungsfaktor / 16 bit timer = 18.73 => 20 => {/4} {/5}
* => ID = 4
* => IDEX = 5
*
* CCR0 = Teilungsfaktor / (ID * IDEX) =  61_377
* TA0CCR0 = 57_540 - 1 = 61_376
* => hex(57_539) = 0xefc0
*/
#define CCR0_res 0xefc0

#pragma FUNC_ALWAYS_INLINE(TA0_init)
GLOBAL Void TA0_init(Void) {
   outerPtr = mA[0];
   ptr = outerPtr;
   TA0CTL   = 0; // stop mode, disable and clear flags
   TA0CCTL0 = 0; // no capture mode, compare mode
                 // clear and disable interrupt flag
   TA0CCR0  = CCR0_res;       // set up Compare Register
   TA0EX0   = TAIDEX_5;     // set up expansion register
   TA0CTL   = TASSEL__ACLK  // 613.75 kHz
            | MC__UP        // Up Mode
            | ID__4         // input divider
            | TACLR         // clear and start Timer
            | TAIE          // enable interrupt
            | TAIFG;        // set interrupt flag
   set_blink_muster(MUSTER1);
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt Void TIMER0_A1_ISR(Void) {

   /*
    * Der Inhalt der ISR ist zu implementieren
    */

    if (*ptr EQ 0) {
        ptr = outerPtr;
    }
    UInt cnt = *ptr++;

    if (TSTBIT(cnt, HIGH)) {
        SETBIT(P1OUT, BIT7);
        CLRBIT(cnt, HIGH);
    } else {
        CLRBIT(P1OUT, BIT7);
    }

   CLRBIT(TA0CTL, TAIFG);
   TA0CCR0 = cnt;
}
