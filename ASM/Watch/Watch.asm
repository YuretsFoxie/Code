.device ATmega8
.include "m8def.inc"

.equ F_CPU = 1000000   ; Define CPU frequency as 1 MHz
.equ SAMPLE_RATE = 256 ; Define the sample rate

; Define the sine wave values in the data segment
.dseg
sine_wave:
    .db 128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 161, 164, 167, 170, 173
    .db 176, 179, 182, 185, 188, 191, 194, 197, 200, 203, 206, 209, 212, 215, 218, 221
    .db 224, 227, 230, 233, 236, 239, 242, 245, 248, 251, 254, 255, 254, 251, 248, 245
    .db 242, 239, 236, 233, 230, 227, 224, 221, 218, 215, 212, 209, 206, 203, 200, 197
    .db 194, 191, 188, 185, 182, 179, 176, 173, 170, 167, 164, 161, 158, 155, 152, 149
    .db 146, 143, 140, 137, 134, 131, 128, 125, 122, 119, 116, 113, 110, 107, 104, 101
    .db  98,  95,  92,  89,  86,  83,  80,  77,  74,  71,  68,  65,  62,  59,  56,  53
    .db  50,  47,  44,  41,  38,  35,  32,  29,  26,  23,  20,  17,  14,  11,   8,   5
    .db   2,   1,   2,   5,   8,  11,  14,  17,  20,  23,  26,  29,  32,  35,  38,  41
    .db  44,  47,  50,  53,  56,  59,  62,  65,  68,  71,  74,  77,  80,  83,  86,  89
    .db  92,  95,  98, 101, 104, 107, 110, 113, 116, 119, 122, 125, 128, 131, 134, 137
    .db 140, 143, 146, 149, 152, 155, 158, 161, 164, 167, 170, 173, 176, 179, 182, 185
    .db 188, 191, 194, 197, 200, 203, 206, 209, 212, 215, 218, 221, 224, 227, 230, 233
    .db 236, 239, 242, 245, 248, 251, 254, 255, 254, 251, 248, 245, 242, 239, 236, 233
    .db 230, 227, 224, 221, 218, 215, 212, 209, 206, 203, 200, 197, 194, 191, 188, 185
    .db 182, 179, 176, 173, 170, 167, 164, 161, 158, 155, 152, 149, 146, 143, 140, 137
    .db 134, 131, 128

.cseg
.org 0x00
rjmp RESET

.org 0x10
RESET:
    ; Define temp register
    .def temp = r16

    ; Initialize stack pointer
    ldi temp, HIGH(RAMEND)
    out SPH, temp
    ldi temp, LOW(RAMEND)
    out SPL, temp

    ; Set PB1 as output (OC1A)
    sbi DDRB, PB1

    ; Set Fast PWM mode with ICR1 as top value
    ldi temp, (1<<WGM11) | (1<<COM1A1)
    out TCCR1A, temp
    ldi temp, (1<<WGM13) | (1<<WGM12) | (1<<CS10)
    out TCCR1B, temp

    ; Set top value for 8-bit resolution
    ldi temp, 0xFF
    out ICR1L, temp
    out ICR1H, temp

    ; Enable global interrupts
    sei

main_loop:
    ; Output sine wave values through PWM
    ldi ZH, HIGH(sine_wave)
    ldi ZL, LOW(sine_wave)
    ldi temp, 0
output_sine_wave:
    ld r0, Z+
    out OCR1AL, r0
    out OCR1AH, r0
    inc temp
    cpi temp, 0xFF
    brne output_sine_wave
    rjmp main_loop
