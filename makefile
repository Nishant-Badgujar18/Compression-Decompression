program : main.o error.o HuffmanC.o HuffmanD.o LZWC.o LZWD.o tree.o dictionary.o report.o
	cc -o program main.o error.o HuffmanC.o HuffmanD.o LZWC.o LZWD.o tree.o dictionary.o report.o

all : main.o error.o HuffmanC.o HuffmanD.o LZWC.o LZWD.o tree.o dictionary.o report.o
	cc -o program main.o error.o HuffmanC.o HuffmanD.o LZWC.o LZWD.o tree.o dictionary.o report.o

main.o : main.c common.h
	cc -c main.c
error.o : error.c common.h
	cc -c error.c
HuffmanC.o : HuffmanC.c common.h 
	cc -c HuffmanC.c
Huffman.o : Huffman.c common.h 
	cc -c Huffman.c
HuffmanD.o : HuffmanD.c common.h
	cc -c HuffmanD.c
LZWC.o : LZWC.c common.h
	cc -c LZWC.c
LZWD.o : LZWD.c common.h
	cc -c LZWD.c
tree.o : tree.c tree.h
	cc -c tree.c
dictionary.o : dictionary.c dictionary.h  common.h
	cc -c dictionary.c
report.o : report.c common.h
	cc -c report.c	
clean :
	rm program main.o error.o HuffmanC.o HuffmanD.o LZWC.o LZWD.o tree.o dictionary.o
