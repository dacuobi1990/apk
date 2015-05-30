all:
	cd match_api;make;cd ..;\
	make apk_plguin.so
apk_plguin.so:helloworld.o match_api/match_api.a
	gcc -o apk_plguin.so -shared -fPIC helloworld.o match_api/match_api.a

helloworld.o:helloworld.c stream.h helloworld.h
	gcc -c -o helloworld.o helloworld.c

clean:
	cd match_api;make clean;cd ..;\
	rm -f helloworld.o
	rm -f apk_plguin.so


