CC = clang++
CFLAGS = -I /usr/lib/llvm-6.0/include/
CLIBS = -lclang

dumpSparrow: cimpAST
	$(CC) $(CFLAGS) $(CLIBS) cimporter.o cimpAST.o dumpSparrow.cpp -o dumpSparrow.out

cimpAST: cimporter.cpp cimpAST.cpp
	$(CC) -c $(CFLAGS) cimporter.cpp cimpAST.cpp

test: dumpSparrow
	python run_tests.py

clean:
	rm -f *.o dumpSparrow.out testFile.tmp
