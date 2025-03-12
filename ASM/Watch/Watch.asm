.device ATmega8
.include "m8def.inc"

.equ frequency	  = 1000000
.equ baudrate     = 4800
.equ uartSetting  = frequency / (16 * baudrate) - 1

.macro Load
ldi R16, @1
out @0, R16
.endm

.macro InvertBits
ldi  R17, @1
in   R16, @0
eor  R16, R17
out  @0, R16
.endm

.macro InitStack
Load SPL, Low(RAMEND)
Load SPH, High(RAMEND)
.endm

.macro InitPorts
sbi DDRB, PB0
sbi DDRB, PB1
sbi DDRB, PB2
cbi PORTB, PB0
sbi PORTB, PB1
cbi PORTB, PB2
.endm

.macro InitUART
Load UBRRL, low(uartSetting)
Load UBRRH, high(uartSetting)
Load UCSRA, 0
Load UCSRB, 1<<RXEN | 1<<TXEN | 1<<RXCIE | 0<<TXCIE | 0<<UDRIE
Load UCSRC, 1<<URSEL | 1<<UCSZ0 | 1<<UCSZ1
.endm

.macro InitTimer
Load TCCR0, (1<<CS02) | (1<<CS00)	; Prescaler 1024
Load TIMSK, (1<<TOIE0)				; Enable overflow interrupt
.endm

.macro DisableUnused
Load ACSR, 1<<ACD
.endm

.macro DisableInterrupts
cli
.endm

.macro EnableInterrupts
sei
.endm

.macro Init
InitStack
InitPorts
InitUART
InitTimer
DisableUnused
EnableInterrupts
.endm

.macro Run
Main:
DisableInterrupts
InvertBits PORTB, 0b00000001
EnableInterrupts
rjmp Main
.endm

;=====

.org 0x0000 rjmp Reset
.org 0x0009	rjmp OnTimer0Overflow
.org 0x000b rjmp OnUARTReceived

;=====

Reset:
Init
Run

OnTimer0Overflow:
InvertBits PORTB, 0b00000110
Load TCNT0, 246					; 50 Hz for prescaler 1024
reti

OnUARTReceived:
in   R16, UDR
inc  R16
out  UDR, R16
reti

;=====
