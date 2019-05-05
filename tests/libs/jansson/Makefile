all: sparrow
	clang -static main.o -L. -ljansson -o jansson_demo.out

sparrow:
	SparrowCompiler -dump-assembly -c main.spr -o main.o

sparrow2:
	SparrowCompiler -dump-assembly main.spr -L. -l libjansson.a -o main.out

clean:
	rm -f *.o example.out
