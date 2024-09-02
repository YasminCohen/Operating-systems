.PHONY: all clean

all: react_server

react_server: react_server.o st_reactor.so
	gcc -o react_server react_server.o ./st_reactor.so -pthread

st_reactor.so: st_reactor.o
	gcc -o st_reactor.so st_reactor.o -pthread -shared

st_reactor.o: st_reactor.c
	gcc -fPIC -c st_reactor.c

react_server.o: react_server.c
	gcc -c react_server.c

clean:
	rm -f *.o *.so react_server