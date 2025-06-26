@echo off
cls
del *.o
del *.prg
echo ---- VIC ----
cl65 -O -t vic20 gemhunt.c utils.c screen.c input.c network.c -o gemhunt20.prg
echo ---- C64 ----
cl65 -O -t c64   gemhunt.c utils.c screen.c input.c network.c -o gemhunt64.prg
echo.
dir *.prg
