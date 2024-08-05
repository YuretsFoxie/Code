.device  ATtiny13A
.include "tn13Adef.inc"

.equ F_SYSTEM			= 4800000
.equ PRESCALER			= 1024
.equ F_MAIN				= 50

; TODO: adjust the values
.equ F_MIN				= 400	; 500
.equ F_MID				= 600	; 750
.equ F_MAX				= 1600	; 1000

.equ MATCH_MAIN			= F_SYSTEM / (F_MAIN * PRESCALER) - 1
.equ MATCH_MIN			= F_SYSTEM / (F_MIN * PRESCALER) - 1
.equ MATCH_MID			= F_SYSTEM / (F_MID * PRESCALER) - 1
.equ MATCH_MAX			= F_SYSTEM / (F_MAX * PRESCALER) - 1

; TODO: adjust the values
.equ ADCThreshold		= 0b11111000
.equ ADCMatchesCount	= 0b01000000
.equ DelayNumber		= 0b00000100

.def DelayCounter		= R17
.def ADCCounter			= R18

;=====

.macro Load
ldi	 R16, @1
out	 @0, R16
.endm

.macro InitStack
Load SPL, low(RAMEND)
.endm

.macro SetIsTurnedRight
set
.endm

.macro ClearIsTurnedRight
clt
.endm

.macro CheckIsTurnedRight
brts @0
.endm

.macro EnableTurnIfNeeded
cpi  DelayCounter, 0
breq ShouldEnable
reti

ShouldEnable:
ldi DelayCounter, DelayNumber
.endm

.macro InitWatchdog
Load WDTCR, 1<<WDTIE | 1<<WDP2 | 1<<WDP0	; Watchdog interrupt is called each 0.5 second, 1<<WDP2 | 1<<WDP1 for 1 second
.endm

.macro InitTimer0
Load OCR0A,  MATCH_MAIN
Load OCR0B,  MATCH_MID
Load TCCR0A, 1<<WGM01						; CTC mode (timer clears on match OCR0A)
Load TCCR0B, 1<<CS02 | 0<<CS01 | 1<<CS00	; prescaler 1024
Load TIMSK0, 1<<OCIE0A | 1<<OCIE0B			; Enable the Compare Match interrupts
Load TIFR0,  0b11111111					; Clear the interrupt flags
.endm

.macro InitADC
Load ADMUX,  0<<REFS0 | 1<<ADLAR | 0<<MUX1 | 1<<MUX0									; Vcc is used as voltage reference, result is left-adjusted, ADC1 (PB2 is input)
Load ADCSRA, 1<<ADEN | 1<<ADSC | 1<<ADATE | 1<<ADIE | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0	; ADC enabled, start conversion, trigger source enabled (for the free running mode), interrupt enabled, prescaler 128
Load ADCSRB, 0<<ADTS2 | 0<<ADTS1 | 0<<ADTS0												; Free running mode enabled
.endm

.macro InitExternalInterrupt
Load MCUCR, 0<<ISC01 | 0<<ISC00
Load GIMSK, 1<<PCIE
Load PCMSK, 1<<PCINT1
.endm

.macro InitPortB
Load DDRB,  0b00010000
Load PORTB, 0b00000110
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
InitWatchdog
InitTimer0
InitADC
InitExternalInterrupt
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
sbi  PINB, PB4

cpi  DelayCounter, DelayNumber
breq TurnLeftOrRight

cpi  DelayCounter, 0
breq ReturnWatchdogTimeout

dec  DelayCounter
reti

TurnLeftOrRight:
dec  DelayCounter
sbi DDRB, PB0

CheckIsTurnedRight TurnLeft
SetIsTurnedRight
Load OCR0B, MATCH_MAX
reti

TurnLeft:
ClearIsTurnedRight
Load OCR0B, MATCH_MIN
reti

ReturnWatchdogTimeout:
Load OCR0B, MATCH_MID
reti

;=====

ADCConversionComplete:
in   R16, ADCH
cpi  R16, ADCThreshold
brlo IsVoltageLess

cpi  ADCCounter, ADCMatchesCount
breq ADCCounterIsFull
inc  ADCCounter
reti

ADCCounterIsFull:
EnableTurnIfNeeded
reti

IsVoltageLess:
cpi  ADCCounter, 0
breq ADCCounterIsEmpty
dec  ADCCounter
reti

ADCCounterIsEmpty:
reti

;=====

/*
onADCConversionComplete()
{
	if (ADCH >= ADCThreshold)
	{
		if (ADCCounter < ADCMatchesCount)
		{
			ADCCounter++;
		}
		else if (ADCCounter == ADCMatchesCount)
		{
			ledOn();
		}
	}
	else
	{
		if (ADCCounter > 0)
		{
			ADCCounter--;
		}
		else if (ADCCounter == 0)
		{
			ledOff();
		}
	}
}
*/
