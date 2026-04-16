#include <msp430.h> 
#include "..\base.h"
#include "event.h"
#include "TA0.h"
#include "TA1.h"

GLOBAL Int _system_pre_init(Void) {
   // stop watchdog timer
   WDTCTL = WDTPW + WDTHOLD;
   return 0;
}

// DCO mit 8.0 MHz
// XT1 mit 4.91 MHz
// ACLK  := XT1 / 8  = 613.75 kHz
// SMCLK := DCO / 32 = 250.0 kHz
// MCLK  := DCO / 1  = 8.0 MHz
#pragma FUNC_ALWAYS_INLINE(CS_init)
LOCAL inline Void CS_init(Void) {
   CSCTL0_H = CSKEY_H;    // enable clock system
   CSCTL1 = DCOFSEL_3;    // DCO frequency = 8.0 MHz
                          // select clock sources
   CSCTL2 = SELA__XT1CLK  // ACLK  <- XT1
          | SELS__DCOCLK  // SMCLK <- DCO
          | SELM__DCOCLK; // MCLK  <- DCO
                          // set frequency divider
   CSCTL3 = DIVA__8       // ACLK  : /8
          | DIVS__32      // SMCLK : /32
          | DIVM__1;      // MCLK  : /1
   CSCTL4 = XT2OFF        // XT2 disabled
          | XTS           // XT1 HF mode
          | XT1DRIVE_0;   // XT1 low power, no bypass
   CSCTL5 = 0;            // disable and clear fault flags
   CSCTL6 = 0;            // disable conditional requests
   CSCTL0_H = 0;          // disable clock system
}

#define VAL_16BIT(arg1, arg2) ((unsigned)(((arg1) << 8) | (arg2)))

#pragma FUNC_ALWAYS_INLINE(GPIO_init)
LOCAL inline Void GPIO_init(Void) {
   // Port 3: Pin 7 => output, TEST4
   // Port 3: Pin 6 => output, TEST3
   // Port 3: Pin 5 => output, TEST2
   // Port 3: Pin 4 => output, TEST1
   // Port 2: Pin 7 => output, LED1
   // Port 1: Pin 2 => output, LED2
   // Port 1: Pin 0 => input,  BTN1
   // Port 1: Pin 1 => input,  BTN2

   //                   Port2       Port1
   //               Bit 76543210    76543210
   PAOUT  = VAL_16BIT(0b00000000, 0b00000000); // clear all outputs
   PADIR  = VAL_16BIT(0b10000000, 0b00000100); // direction, set outputs
   PAREN  = VAL_16BIT(0b00000000, 0b00000000); // without pull up
   PAIFG  = VAL_16BIT(0b00000000, 0b00000000); // clear all interrupt flags
   PAIES  = VAL_16BIT(0b00000000, 0b00000000);
   PAIE   = VAL_16BIT(0b00000000, 0b00000000); // disable all GPIO interrupts
   PASEL0 = VAL_16BIT(0b00000000, 0b00000000);
   PASEL1 = VAL_16BIT(0b00000000, 0b00000000);

   //                   Port4       Port3
   //               Bit 76543210    76543210
   PBOUT  = VAL_16BIT(0b00000000, 0b10000000); // clear all outputs
   PBDIR  = VAL_16BIT(0b00000000, 0b11110000); // direction, set outputs
   PBREN  = VAL_16BIT(0b00000000, 0b00000000); // without pull up
   PBIFG  = VAL_16BIT(0b00000000, 0b00000000); // clear all interrupt flags
   PBIES  = VAL_16BIT(0b00000000, 0b00000000);
   PBIE   = VAL_16BIT(0b00000000, 0b00000000); // disable all GPIO interrupts
   PBSEL0 = VAL_16BIT(0b00000000, 0b00000000);
   PBSEL1 = VAL_16BIT(0b00000000, 0b00000000);
}

GLOBAL Void main(Void) {
   Int cnt = MUSTER1;

   GPIO_init();   // set up Ports
   CS_init();     // set up Clock System
   Event_init();  // set up Events
   TA0_init();    // set up Timer A0
   TA1_init();    // set up Timer A1

   while(TRUE) {
      Event_wait();

      if (Event_tst(EVENT_BTN2)) {
         Event_clr(EVENT_BTN2);
         if (++cnt GT MUSTER6) {
            cnt = MUSTER1;
         }
         set_blink_muster(cnt);
      }
      if (Event_tst(EVENT_BTN1)) {
         Event_clr(EVENT_BTN1);
         TGLBIT(P2OUT, BIT7);
      }
      if (Event_err()) {
         TA0CTL = 0; // stop mode, disable and clear flags
         SETBIT(P1OUT, BIT2); // LED on
      }
   }
}

