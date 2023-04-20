.PHONY = all clean
#Defining Macros
CC = gcc
FLAGS = -Wall -g0
FPIC = -fpic
SH = -shared

all: stshell copy cmp codecA codecB encode decode

#Creating Programs (tools)
stshell: stshell.c
	$(CC) $(FLAGS) stshell.c -o stshell
copy: copy.c
	$(CC) $(FLAGS) copy.c -o copy
cmp: cmp.c
	$(CC) $(FLAGS) cmp.c -o cmp
encode: encode.c
	$(CC) $(FLAGS) encode.c -o encode -ldl
decode: decode.c
	$(CC) $(FLAGS) decode.c -o decode -ldl

#Creating 2 Shared Libraries
codecA: codecA.c codec.h
	$(CC) $(FLAGS) -o libcodecA.so $(SH) $(FPIC) codecA.c
codecB: codecB.c codec.h
	$(CC) $(FLAGS) -o libcodecB.so $(SH) $(FPIC) codecB.c

clean:
	rm -f *.so stshell copy cmp encode decode