@echo off
cls
del *.prg
echo ---- VIC ----
cl65 -O -t vic20 gemhunt.c utils.c screen.c -o gemhunt20.prg
echo ---- C64 ----
cl65 -O -t c64   gemhunt.c utils.c screen.c -o gemhunt64.prg
dir *.prg
