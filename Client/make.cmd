@echo off
cls
del *.prg
cl65 -O -t vic20 gemhunt.c -D PLATFORM=VIC -o gemhunt20.prg
cl65 -O -t c64 gemhunt.c -D PLATFORM=C64 -o gemhunt64.prg
dir *.prg
