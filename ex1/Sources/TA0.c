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
#define TICK(t) ((UInt)(((ACKFRQ * t) / 8.0) / 6.0) - 1)
#define T_TICK 500

#define M1_SIZE 3
#define M2_SIZE 3
#define M3_SIZE 3
#define M4_SIZE 4
#define M5_SIZE 6
#define M6_SIZE 8
#define TABSIZE 7

LOCAL const UInt m1[M1_SIZE] = {
    HIGH | TICK(T_TICK * 4),
    LOW  | TICK(T_TICK * 1),
    0
};
LOCAL const UInt m2[M2_SIZE] = {
    HIGH | TICK(T_TICK * 15e-1),
    LOW  | TICK(T_TICK * 15e-1),
    0
};
LOCAL const UInt m3[M3_SIZE] = {
    HIGH | TICK(T_TICK * 5e-1),
    LOW  | TICK(T_TICK * 5e-1),
    0
};
LOCAL const UInt m4[M4_SIZE] = {
    LOW  | TICK(T_TICK * 1),
    HIGH | TICK(T_TICK * 1),
    LOW  | TICK(T_TICK * 5),
    0
};
LOCAL const UInt m5[M5_SIZE] = {
    LOW  | TICK(T_TICK * 1),
    HIGH | TICK(T_TICK * 1),
    LOW  | TICK(T_TICK * 1),
    HIGH | TICK(T_TICK * 1),
    LOW  | TICK(T_TICK * 5),
    0
};
LOCAL const UInt m6[M6_SIZE] = {
    LOW  | TICK(T_TICK * 1),
    HIGH | TICK(T_TICK * 1),
    LOW  | TICK(T_TICK * 1),
    HIGH | TICK(T_TICK * 1),
    LOW  | TICK(T_TICK * 1),
    HIGH | TICK(T_TICK * 1),
    LOW  | TICK(T_TICK * 5),
    0
};

LOCAL const UInt* mA[TABSIZE] = {
    m1,
    m2,
    m3,
    m4,
    m5,
    m6,
    0
};

LOCAL volatile const UInt* outerPtr;
LOCAL volatile const UInt* ptr;

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
* timespan = 4500 ms
* Timer_clock = = 613.75 kHz =ACKFRQ
* Teilungsfaktor = Timer_CLOCK * timespan = 2_761_875
*
* 16 Bit timer : 2**16
* Skalierungsfaktor = Teilungsfaktor / 16 bit timer = 42.143 => {/8} {/6}
* => ID = 8
* => IDEX = 6
*
* CCR0 = Teilungsfaktor / (ID * IDEX) =  57_540
* TA0CCR0 = 57_540 - 1 = 57_539
* => hex(57_539) = 0xe0c3
*/
#define CCR0_res 0xe0c3

#pragma FUNC_ALWAYS_INLINE(TA0_init)
GLOBAL Void TA0_init(Void) {
   outerPtr = mA[0];
   ptr = outerPtr;
   TA0CTL   = 0; // stop mode, disable and clear flags
   TA0CCTL0 = 0; // no capture mode, compare mode
                 // clear and disable interrupt flag
   TA0CCR0  = CCR0_res;       // set up Compare Register
   TA0EX0   = TAIDEX_6;     // set up expansion register
   TA0CTL   = TASSEL__ACLK  // 613.75 kHz
            | MC__UP        // Up Mode
            | ID__8         // input divider
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
        SETBIT(P1OUT, BIT2);
        CLRBIT(cnt, HIGH);
    } else {
        CLRBIT(P1OUT, BIT2);
    }

   CLRBIT(TA0CTL, TAIFG);
   TA0CCR0 = cnt;
}
