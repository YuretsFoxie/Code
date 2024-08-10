avrdude -C "C:\Data\Tools\AVRDude\avrdude.conf" -c ftbb -p t13 -P ft0 -B 2400 -U hfuse:r:hfuse_tiny.txt:i -U lfuse:r:lfuse_tiny.txt:i
pause
