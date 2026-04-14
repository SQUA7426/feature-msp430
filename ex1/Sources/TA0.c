#include <msp430.h>
#include "../base.h"
#include "TA0.h"

/*
 * Man soll sich eine geeignete Datenstruktur �berlegen,
 * die eine laufzeiteffiziente Ausf�hrung der ISR erm�glicht.
 *
*/
#define HIGH    0x8000
#define LOW     0x0000

#define ACKFRQ  613.75 // kHz
#define TICK(t) ((Int)(((ACKFRQ * t) / 1.0) / 5.0) - 1)
#define T_TICK 250

#define M1_SIZE 3
#define M2_SIZE 3
#define M3_SIZE 3
#define M4_SIZE 4
#define M5_SIZE 6
#define M6_SIZE 8
#define TABSIZE 7

LOCAL const int m1[M1_SIZE] = {
    HIGH | TICK(T_TICK * 8),
    LOW  | TICK(T_TICK * 2),
    0
};
LOCAL const int m2[M2_SIZE] = {
    HIGH | TICK(T_TICK * 3),
    LOW  | TICK(T_TICK * 3),
    0
};
LOCAL const int m3[M3_SIZE] = {
    HIGH | TICK(T_TICK * 1),
    LOW  | TICK(T_TICK * 1),
    0
};
LOCAL const int m4[M4_SIZE] = {
    LOW  | TICK(T_TICK * 2),
    HIGH | TICK(T_TICK * 2),
    LOW  | TICK(T_TICK * 10),
    0
};
LOCAL const int m5[M5_SIZE] = {
    LOW  | TICK(T_TICK * 2),
    HIGH | TICK(T_TICK * 2),
    LOW  | TICK(T_TICK * 2),
    HIGH | TICK(T_TICK * 2),
    LOW  | TICK(T_TICK * 10),
    0
};
LOCAL const int m6[M6_SIZE] = {
    LOW  | TICK(T_TICK * 2),
    HIGH | TICK(T_TICK * 2),
    LOW  | TICK(T_TICK * 2),
    HIGH | TICK(T_TICK * 2),
    LOW  | TICK(T_TICK * 2),
    HIGH | TICK(T_TICK * 2),
    LOW  | TICK(T_TICK * 10),
    0
};
/*
#define M1 {HIGH | TICK(T_TICK * 8), LOW  | TICK(T_TICK * 2), 0}
#define M2 {HIGH | TICK(T_TICK * 3), LOW  | TICK(T_TICK * 3),0}
#define M3 {HIGH | TICK(T_TICK * 1), LOW  | TICK(T_TICK * 1),0}
#define M4 {LOW  | TICK(T_TICK * 2), HIGH | TICK(T_TICK * 2), LOW  | TICK(T_TICK * 10),0}
#define M5 {LOW  | TICK(T_TICK * 2), HIGH | TICK(T_TICK * 2), LOW  | TICK(T_TICK * 2), HIGH | TICK(T_TICK * 2), LOW  | TICK(T_TICK * 10), 0}
#define M6 {LOW  | TICK(T_TICK * 2), HIGH | TICK(T_TICK * 2), LOW  | TICK(T_TICK * 2), HIGH | TICK(T_TICK * 2), LOW  | TICK(T_TICK * 2), HIGH | TICK(T_TICK * 2), LOW  | TICK(T_TICK * 10),0}
*/
LOCAL const int* mA[TABSIZE] = {
    m1,
    m2,
    m3,
    m4,
    m5,
    m6,
    0
};

LOCAL const int** outerPtr;
LOCAL const int* ptr = 0;

GLOBAL Void set_blink_muster(UInt arg) {
/*
 * Die Funktion muss so erweitert werden,
 * dass ein Blinkmuster selektiert wird.
 * Diese L�sung h�ngt stark von der gew�hlten
 * Datenstruktur ab.
 */
 // EVENT_BTN2 => modulo-Zaehler
    if (**(outerPtr) LT BIT0) {
        outerPtr = &mA[0];
    }
}

/*
* times = [2500, 1500, 500, 2500, 3500, 4500] (ms)
* timespan = 250 ms
* Timer_clock = = 613.75 kHz =ACKFRQ
* Teilungsfaktor = Timer_CLOCK * timespan = 153_427.5
*
* 16 Bit timer : 2**15
* Skalierungsfaktor = Teilungsfaktor / 16 bit timer = 4.6825 => 5
* => ID = 1
* => IDEX = 5
*
* CCR0 = Teilungsfaktor / (ID * IDEX) =  30_687
* TA0CCR0 = 30_687 - 1
* => hex(30_686) = 0x77de
*/
#define CCR0_res 0x77de

#pragma FUNC_ALWAYS_INLINE(TA0_init)
GLOBAL Void TA0_init(Void) {
   outerPtr = &mA[0];
   ptr = &outerPtr[0];
   TA0CTL   = 0; // stop mode, disable and clear flags
   TA0CCTL0 = 0; // no capture mode, compare mode
                 // clear and disable interrupt flag
   TA0CCR0  = CCR0_res;       // set up Compare Register
   TA0EX0   = TAIDEX_5;     // set up expansion register
   TA0CTL   = TASSEL__ACLK  // 613.75 kHz
            | MC__UP        // Up Mode
            | ID__1         // input divider
            | TACLR         // clear and start Timer
            | TAIE          // enable interrupt
            | TAIFG;        // set interrupt flag
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt Void TIMER0_A1_ISR(Void) {

   /*
    * Der Inhalt der ISR ist zu implementieren
    */

    if (*ptr EQ 0) {
        ptr = &outerPtr[0];
    }

    UInt cnt = *ptr++;

    if (TSTBIT(cnt, HIGH)) {
        SETBIT(P2OUT, BIT7);
        CLRBIT(cnt, HIGH);
    } else {
        CLRBIT(P2OUT, BIT7);
    }

   CLRBIT(TA0CTL, TAIFG);
   TA0CCR0 = cnt;
}
