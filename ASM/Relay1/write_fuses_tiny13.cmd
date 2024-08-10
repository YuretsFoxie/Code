avrdude -C "C:\Data\Tools\AVRDude\avrdude.conf" -c ftbb -p t13 -P ft0 -B 1200 -U lfuse:w:0x65:m -U hfuse:w:0xff:m
pause
