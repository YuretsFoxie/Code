.device ATtiny2313
.include "tn2313def.inc"

.equ XTAL	      = 8000000
.equ baudrate     = 9600
.equ prescaler    = 256
.equ uartSetting  = XTAL / (16 * baudrate) - 1
.equ timerSetting = XTAL / prescaler

;=====

.macro Load
ldi  R16, @1
out  @0, R16
.endm

.macro InvertBits
ldi  R17, @1
in   R16, @0
eor  R16, R17
out  @0, R16
.endm

.macro InitStack
Load SPL, low(RAMEND)
.endm

.macro InitPortB
Load DDRB,  0b11111111
Load PORTB, 0b11111111 ;The LED is turned ON by 0 and turned OFF by 1
.endm

.macro InitUART
Load UBRRL, low(uartSetting)
Load UBRRH, high(uartSetting)
Load UCSRA, 0
Load UCSRB, 1<<RXEN | 1<<TXEN | 1<<RXCIE | 0<<TXCIE | 0<<UDRIE
Load UCSRC, 1<<UCSZ0 | 1<<UCSZ1
.endm

.macro InitTimer
Load OCR1AL, low(timerSetting)
Load OCR1AH, high(timerSetting)
Load TCNT1H, 0
Load TCNT1L, 0
Load TCCR1B, 0b00001100 ; enable the timer 1 with prescaler 256
Load TIMSK, 1<<OCIE1A
.endm

.macro DisableUnused
Load ACSR, 1<<ACD
Load TIFR, 0xFF
Load GIFR, 0xFF
.endm

.macro Init
InitStack
InitPortB
InitUART
InitTimer
DisableUnused
sei
.endm

.macro Run
Main: rjmp  Main
.endm

;=====

.org 0x0000 rjmp Reset
.org 0x0006 rjmp TimerCompareMatch
.org 0x0007 rjmp UARTReceivingIsCompleted

;=====

Reset:
Init
Run

;=====

TimerCompareMatch:
InvertBits PORTB, 0b00111100
reti

UARTReceivingIsCompleted:
in   R16, UDR
inc  R16
out  UDR, R16
reti

;=====
