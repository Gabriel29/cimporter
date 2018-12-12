CC = clang++
CFLAGS = -I /usr/include/
CLIBS = -lclang

dumpSparrow: cimpAST
	$(CC) $(CFLAGS) $(CLIBS) cimporter.o cimpAST.o utils.o dumpSparrow.cpp -o dumpSparrow.out

cimpAST: cimporter.cpp cimpAST.cpp utils.cpp
	$(CC) -c $(CFLAGS) cimporter.cpp cimpAST.cpp utils.cpp

test: dumpSparrow
	python run_tests.py

clean:
	rm -f *.o dumpSparrow.out testFile.tmp
