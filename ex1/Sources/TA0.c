#include <msp430.h>
#include "..\base.h"
#include "TA0.h"

/*
 * Man soll sich eine geeignete Datenstruktur �berlegen,
 * die eine laufzeiteffiziente Ausf�hrung der ISR erm�glicht.
 *
*
* times = [2500, 1500, 500, 2500, 3500, 4500] (ms)
* timespan = 4500 ms
* Timer_clock = = 613.75 kHz =ACKFRQ
* Teilungsfaktor = Timer_CLOCK * timespan = 1_227_500
*
* 16 Bit timer : 2**15
* Skalierungsfaktor = Teilungsfaktor / 16 bit timer = 37.46 => 40 => {/8} {/5}
* => ID = 8
* => IDEX = 5
*
* CCR0 = Teilungsfaktor / (ID * IDEX) = 30_688
* TA0CCR0 = 30_688 - 1 = 30_687
* => hex(30_687) = 0x77df
*/
#define CCR0_res 0x77df

#define HIGH    0x8000
#define LOW     0x0000

#define ACKFRQ  613.75 // kHz
#define TICK(t) ((UInt)(((ACKFRQ * t) / 8.0) / 5.0) - 1)

#define MASIZE 27
#define TABSIZE 6

#define S1 0
#define S2 3
#define S3 6
#define S4 9
#define S5 13
#define S6 19

#define T1 250
#define T2 500
#define T3 750
#define T4 1500
#define T5 2000

LOCAL const UInt ma[MASIZE] = {
    HIGH | TICK(T5),  // 0
    LOW  | TICK(T2),
    0,
    //};
    //LOCAL const UInt m2[] = {
    HIGH | TICK(T3),   // 3
    LOW  | TICK(T3),
    0,
    //};
    //LOCAL const UInt m3[] = {
    HIGH | TICK(T1),   // 6
    LOW  | TICK(T1),
    0,
    //};
    //LOCAL const UInt m4[] = {
    LOW  | TICK(T2),   // 9
    HIGH | TICK(T2),
    LOW  | TICK(T4),
    0,
    //};
    //LOCAL const UInt m5[] = {
    LOW  | TICK(T2),   // 13
    HIGH | TICK(T2),
    LOW  | TICK(T2),
    HIGH | TICK(T2),
    LOW  | TICK(T4),
    0,
    //};
    //LOCAL const UInt m6[] = {
    LOW  | TICK(T2),   // 19
    HIGH | TICK(T2),
    LOW  | TICK(T2),
    HIGH | TICK(T2),
    LOW  | TICK(T2),
    HIGH | TICK(T2),
    LOW  | TICK(T4),
    0
};

LOCAL const UChar startpts[TABSIZE] = {
    S1, S2, S3, S4, S5, S6
};

LOCAL UChar start;
LOCAL const UInt *ptr;

GLOBAL Void set_blink_muster(UInt arg) {
/*
 * Die Funktion muss so erweitert werden,
 * dass ein Blinkmuster selektiert wird.
 * Diese L�sung h�ngt stark von der gew�hlten
 * Datenstruktur ab.
 */
    start = (arg GT TABSIZE -1) ? startpts[S1] : startpts[arg];
}

#pragma FUNC_ALWAYS_INLINE(TA0_init)
GLOBAL Void TA0_init(Void) {
   start = startpts[S1];
   ptr = &ma[start];
   TA0CTL   = 0; // stop mode, disable and clear flags
   TA0CCTL0 = 0; // no capture mode, compare mode
                 // clear and disable interrupt flag
   TA0CCR0  = CCR0_res;       // set up Compare Register
   TA0EX0   = TAIDEX_4;     // set up expansion register 5
   TA0CTL   = TASSEL__ACLK  // 613.75 kHz
            | MC__UP        // Up Mode
            | ID__8         // input divider 8
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
        ptr = &ma[start];
    }

    UInt cnt = *ptr++;

    if (TSTBIT(cnt, HIGH)) {
        SETBIT(P1OUT, BIT2);
        CLRBIT(cnt, HIGH);
    } else {
        CLRBIT(P1OUT, BIT2);
    }

   CLRBIT(TA0CTL, TAIFG);
   TA0CCR0 = cnt;
}
