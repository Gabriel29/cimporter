CC = clang++
CFLAGS = -I /usr/lib/llvm-6.0/include/ -ggdb3 -O0
CLIBS = -lclang

dumpSparrow: cimpAST
	$(CC) $(CFLAGS) $(CLIBS) cimporter.o cimpAST.o utils.o cimpMacroParser.o dumpSparrow.cpp -o dumpSparrow.out

cimpAST: cimporter.cpp cimpAST.cpp utils.cpp cimpMacroParser.cpp
	$(CC) -c $(CFLAGS) cimporter.cpp cimpAST.cpp utils.cpp cimpMacroParser.cpp

test: dumpSparrow
	python run_tests.py

clean:
	rm -f *.o dumpSparrow.out testFile.tmp
