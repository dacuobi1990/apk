match_api.a: match.o read_ini.o picohttpparser.o
	ar -crv match_api.a match.o read_ini.o picohttpparser.o

match.o: match.c match.h struct.h
	gcc -c -o match.o match.c

read_ini.o:read_ini.c struct.h
	gcc -c -o read_ini.o read_ini.c

picohttpparser.o:pico/picohttpparser.c pico/picohttpparser.h
	gcc -c -o picohttpparser.o pico/picohttpparser.c

clean:
	rm -f match_api.a
	rm -f match.o
	rm -f read_ini.o
	rm -f picohttpparser.o
