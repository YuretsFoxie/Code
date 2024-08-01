.device  ATtiny13A
.include "tn13Adef.inc"



;=====

.macro Load
ldi	 R16, @1
out	 @0, R16
.endm

.macro SetBits
in  R16, @0
ori R16, @1
out @0, R16
.endm

.macro InitStack
Load SPL, low(RAMEND)
.endm

.macro InitWatchdog
Load WDTCR, 1<<WDTIE | 1<<WDP2 | 1<<WDP0	; Watchdog interrupt is called each 0.5 second, 1<<WDP2 | 1<<WDP1 for 1 second
.endm

.macro InitTimer0

.endm

.macro InitADC
Load ADMUX, 0<<REFS0 | 1<<ADLAR | 0<<MUX1 | 1<<MUX0										; Vcc is used as voltage reference, result is left-adjusted, ADC1 (PB2 is input)
Load ADCSRA, 1<<ADEN | 1<<ADSC | 1<<ADATE | 1<<ADIE | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0	; ADC enabled, start conversion, trigger source enabled (for the free running mode), interrupt enabled, prescaler 128
Load ADCSRB, 0<<ADTS2 | 0<<ADTS1 | 0<<ADTS0												; Free running mode enabled
.endm

.macro InitExternalInterrupt
Load MCUCR, 0<<ISC01 | 0<<ISC00
Load GIMSK, 1<<PCIE
Load PCMSK, 1<<PCINT1
.endm

.macro InitPortB
;Load DDRB,  0b11111101		; PB1 - input
;Load PORTB, 0b00000010

;sbi DDRB, PB0
.endm

.macro DisableComparator
Load ACSR, 1<<ACD | 0<<ACIE
.endm

.macro EnableInterrupts
sei
.endm

;=====

.macro Init
InitStack
;InitWatchdog

InitTimer0

;InitADC
;InitExternalInterrupt

InitPortB
DisableComparator
EnableInterrupts
.endm

.macro Run
Main:
rjmp Main
.endm

;=====

.org 0x0000 rjmp Reset
.org 0x0002 rjmp ExternalInterrupt		; 1 for INT0, 2 for PCINTs
.org 0x0006 rjmp TimerCompareMatchA
.org 0x0008 rjmp WatchdogTimeout
.org 0x0009 rjmp ADCConversionComplete

;=====

Reset:
Init
Run

;=====

ExternalInterrupt:
sbi PORTB, PB4
reti

;=====

TimerCompareMatchA:

reti

;=====

WatchdogTimeout:
; sbi PINB, PB4
reti

;=====

ADCConversionComplete:
reti

;=====

; 3 - LED (PB4, Watchdog)
; 5 - servomotor (PB0, PWM)
; 6 - external interrupt (INT0)
; 7 - photoresistor (ADC1)

; Watchdog is always on, it calls an interrupt each half of a second.
; Timer 0 drives a servomotor with PWM.
; ADC checks a photoresistor
; External interrupt checks a button

; Led always blinks
; Servomotor is set to 0 by default
; If the button is pressed or the resistor is lighted the first time, servomotor turns to -90 and returns to 0 in 2 seconds
; If the button is pressed or the resistor is lighted the second time, servomotor turns to 90 and returns to 0 in 2 seconds

; DDxn PORTxn
; 0    0       input     hi-z
; 0    1       input     pull-up
; 1    0       output    low
; 1    1       output    high
