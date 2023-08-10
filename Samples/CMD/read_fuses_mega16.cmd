avrdude -C "C:\Data\Tools\AVRDude\avrdude.conf" -c ftbb -p m16 -P ft0 -B 2400 -U hfuse:r:hfuse_mega.txt:i -U lfuse:r:lfuse_mega.txt:i
pause
