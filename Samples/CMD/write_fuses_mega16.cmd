avrdude -C "C:\Data\Tools\AVRDude\avrdude.conf" -c ftbb -p m16 -P ft0 -B 1200 -U lfuse:w:0xff:m -U hfuse:w:0xc7:m
pause
