
all: PartA PartB PartC


PartA: cmp copy

cmp: cmp.c
	gcc -Wall -Wextra -Werror -pedantic cmp.c -o cmp

copy: copy.c
	gcc -Wall -Wextra -Werror -pedantic copy.c -o copy



PartB: encode decode

encode:	encode.c lib_A lib_B 
	gcc encode.c -L. -Wl,-rpath,. -lCodecA -lCodecB -o encode

decode:	decode.c lib_A lib_B 
	gcc decode.c -L. -Wl,-rpath,. -lCodecA -lCodecB -o decode

lib_A:	codecA.c
	gcc -shared codecA.c -fPIC -o libCodecA.so

lib_B:	codecB.c
	gcc -shared codecB.c -fPIC -o libCodecB.so


PartC: stshell

stshell: stshell.c
	gcc -Wall -Wextra -Werror -pedantic stshell.c -o stshell


.PHONY: clean all

clean:
	-rm -f *.so *.o cmp copy decode encode stshell