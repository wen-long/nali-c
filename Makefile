all: nali

nali: qqwry.o main.o
	gcc qqwry.o main.o -liconv -o nali

qqwry.o: qqwry.c
	gcc -c qqwry.c

main.o: main.c
	gcc -c main.c

clean:
	rm *.o nali

install:
	cp ./nali /usr/local/bin/nali
	cp ./nali-update /usr/local/bin/nali-update