.device  ATtiny13A
.include "tn13Adef.inc"

.equ F_SYSTEM		= 4800000
.equ PRESCALER		= 1024
.equ F_MAIN			= 50
.equ F_MIN			= 400	; 500
.equ F_MID			= 600	; 750
.equ F_MAX			= 1600	; 1000
.equ MATCH_MAIN		= F_SYSTEM / (F_MAIN * PRESCALER) - 1
.equ MATCH_MIN		= F_SYSTEM / (F_MIN * PRESCALER) - 1
.equ MATCH_MID		= F_SYSTEM / (F_MID * PRESCALER) - 1
.equ MATCH_MAX		= F_SYSTEM / (F_MAX * PRESCALER) - 1
.equ ADCThreshold	= 0b11111100
.equ DelayNumber	= 0b00000100

.def DelayCounter	= R18
.def Flags			= R19

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

.macro ClearBits
in R16, @0
ldi R17, @1
com R17
and R16, R17
out @0, R16
.endm

;=====

.macro SetIsTurnedMax
sbr Flags, 1<<0
.endm

.macro ClearIsTurnedMax
cbr Flags, 1<<0
.endm

.macro CheckIsTurnedMax
sbrc Flags, 0 							; skip if bit 0 clear (if set, there will be a jump)
rjmp @0
.endm

.macro SetIsTurnedMid
sbr Flags, 1<<1
.endm

.macro ClearIsTurnedMid
cbr Flags, 1<<1
.endm

.macro CheckIsTurnedMid
sbrc Flags, 1 							; skip if bit 1 clear (if set, there will be a jump)
rjmp @0
.endm

.macro EnableSleepMode
SetBits MCUCR, 1<<SE
.endm

.macro DisableSleepMode
ClearBits MCUCR, 1<<SE
.endm

.macro EnableTurnIfNeeded
cpi  DelayCounter, 0
breq ShouldEnable
reti

ShouldEnable:
ldi  DelayCounter, DelayNumber
.endm

.macro EnablePWM
sbi DDRB, PB0
.endm

.macro DisablePWM
cbi DDRB, PB0
.endm

;=====

.macro InitStack
Load SPL, low(RAMEND)
.endm

.macro InitWatchdog
Load WDTCR, 1<<WDTIE | 1<<WDP2 | 1<<WDP0	; Watchdog interrupt is called each 0.5 second
.endm

.macro InitTimer0
Load OCR0A,  MATCH_MAIN
Load OCR0B,  MATCH_MID
Load TCCR0A, 1<<WGM01						; CTC mode (timer clears on match OCR0A)
Load TCCR0B, 1<<CS02 | 0<<CS01 | 1<<CS00	; prescaler 1024
Load TIMSK0, 1<<OCIE0A | 1<<OCIE0B			; Enable the Compare Match interrupts
.endm

.macro InitADC
Load ADMUX,  0<<REFS0 | 1<<ADLAR | 0<<MUX1 | 1<<MUX0									; Vcc is used as voltage reference, result is left-adjusted, ADC1 (PB2 is input)
Load ADCSRA, 1<<ADEN | 0<<ADSC | 0<<ADATE | 1<<ADIE | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0	; ADC enabled, don't start conversion yet, trigger source disabled, interrupt enabled, prescaler 128
.endm

.macro InitExternalInterrupt
Load MCUCR, 0<<ISC01 | 0<<ISC00
Load GIMSK, 1<<PCIE
Load PCMSK, 1<<PCINT1
.endm

.macro InitPortB							; PB0 is PWM output, PB1 is button input, PB2 is ADC input, PB4 is LED output
Load DDRB,  0b00010001
Load PORTB, 0b00000110
.endm

.macro ClearRegisters
clr DelayCounter
clr Flags
Load TIFR0, 0b11111111
.endm

.macro DisableComparator
Load ACSR, 1<<ACD | 0<<ACIE
.endm

.macro InitSleep
SetBits MCUCR, 1<<SM0						; Enable Noise Reduction mode
.endm

.macro EnableInterrupts
sei
.endm

;=====

.macro Init
InitStack
InitWatchdog
InitTimer0
InitADC
InitExternalInterrupt
InitPortB
ClearRegisters
DisableComparator
InitSleep
EnableInterrupts
.endm

.macro Run
Main: 
sleep
rjmp Main
.endm

;=====

.org 0x0000 rjmp Reset
.org 0x0002 rjmp ExternalInterrupt			; 1 for INT0, 2 for PCINTs
.org 0x0006 rjmp TimerCompareMatchA
.org 0x0007 rjmp TimerCompareMatchB
.org 0x0008 rjmp WatchdogTimeout
.org 0x0009 rjmp ADCConversionComplete

;=====

Reset:
Init
Run

;=====

ExternalInterrupt:
EnableTurnIfNeeded
reti

;=====

TimerCompareMatchA:
sbi PORTB, PB0
reti

;=====

TimerCompareMatchB:
cbi PORTB, PB0
reti

;=====

WatchdogTimeout:
DisablePWM
sbi  PINB, PB4

cpi  DelayCounter, DelayNumber
breq IsFull

cpi  DelayCounter, 0
breq IsEmpty

dec  DelayCounter
reti

IsFull:
EnablePWM
SetIsTurnedMid
dec DelayCounter
CheckIsTurnedMax IsTurnedMax
SetIsTurnedMax
Load OCR0B, MATCH_MAX
reti

IsTurnedMax:
ClearIsTurnedMax
Load OCR0B, MATCH_MIN
reti

IsEmpty:
EnableSleepMode
CheckIsTurnedMid IsTurnedMid
reti

IsTurnedMid:
ClearIsTurnedMid
EnablePWM
Load OCR0B, MATCH_MID
reti

;=====

ADCConversionComplete:
DisableSleepMode
in   R16, ADCH
cpi  R16, ADCThreshold
brsh IsLight
reti

IsLight:
EnableTurnIfNeeded
reti

;=====

; TODO: allow the next turn only if there was a pause in lighting
