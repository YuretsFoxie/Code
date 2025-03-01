avrdude -C "C:\Data\Tools\AVRDude\avrdude.conf" -c ftbb -p m8 -P ft0 -B 1200 -U lfuse:w:0xe1:m -U hfuse:w:0xd9:m
pause

:: Default values, F = 1 MHz, internal oscillator.
