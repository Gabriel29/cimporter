all: sparrow
	gcc -static main.o -L. -ljansson -o example.out

sparrow:
	SparrowCompiler -dump-assembly -c main.spr -o main.o

clean:
	rm -f *.o example.out
