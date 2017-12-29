# 
# Projekt IFJ - Interpret jazyka IFJ14
#
# Autori: Matúš Bútora
#	  Daniel Dušek
#	  Roman Jaška
#	  Filip Kalous
#	 a Sochor ne.
#
# Datum:  14.12.2014
#

CC=gcc										# prekladac jazyka C
CFLAGS=-std=c99     # parametry programu


main: main.c
	$(CC) $(CFLAGS) main.c -o main
