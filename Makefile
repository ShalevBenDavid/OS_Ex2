.PHONY = all clean
#Defining Macros
CC = gcc
FLAGS = -Wall -g
FPIC = -fpic
SH = -shared

all: copy cmp codecA codecB encode decode

#Creating Programs (tools)
copy:copy.c
	$(CC) $(FLAGS) copy.c -o copy
cmp: cmp.c
	$(CC) $(FLAGS) cmp.c -o cmp
encode: encode.c
	$(CC) $(FLAGS) encode.c -o encode
decode: decode.c
	$(CC) $(FLAGS) decode.c -o decode

#Creating 2 Shared Libraries
codecA: codecA.c codec.h
	$(CC) $(FLAGS) -o libcodecA.so $(SH) $(FPIC) codecA.c
codecB: codecB.c codec.h
	$(CC) $(FLAGS) -o libcodecB.so $(SH) $(FPIC) codecB.c

clean:
	rm -f *.so copy cmp encode decode