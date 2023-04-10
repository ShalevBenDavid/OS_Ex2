.PHONY = all clean
#Defining Macros
CC = gcc
FLAGS = -Wall -g
FPIC = -fpic
SH = -shared

all: codecA codecB encode decode

#Creating Programs (tools)
encode: encode.c
	$(CC) $(FLAGS) encode.c -L. -l codecA -l codecB -o encdoe
decode: decode.c
	$(CC) $(FLAGS) decode.c -L. -l codecA -l codecB -o decode

#Creating 2 Shared Libraries
codecA: codecA.so
	$(CC) $(FLAGS) -o libcodecA.so $(SH) $(FPIC) codecA.c
codecB: codecB.so
	$(CC) $(FLAGS) -o libcodecB.so $(SH) $(FPIC) codecB.c

clean:
	rm -f