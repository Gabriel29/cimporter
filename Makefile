CC = clang++
CFLAGS = -lclang

dumpSparrow: cimpAST
	$(CC) $(CFLAGS) cimporter.o cimpAST.o dumpSparrow.cpp -o dumpSparrow.out

cimpAST: cimporter.cpp cimpAST.cpp
	$(CC) -c $(CFLAGS) cimporter.cpp cimpAST.cpp

clean:
	rm -r *.o genSparrow.out
