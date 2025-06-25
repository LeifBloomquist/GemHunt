@echo off
cls
del *.prg
cl65 -O -t vic20 gemhunt.c utils.c -o gemhunt20.prg
cl65 -O -t c64   gemhunt.c utils.c -o gemhunt64.prg
dir *.prg
