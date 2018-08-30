#include <iostream>
#include "cimpAST.hpp"
#include "dumpSparrow.hpp"

int main(int argc, char** argv)
{
	if (argc < 2 )
	{
		std::cout << "file argument missing!" << std::endl;
		return -1;
	}

	std::string fileName(argv[1]);
	cimporter::File* file = new cimporter::File(fileName);
	file->parseFile();

	/* dump Sparrow code to screen */
	cimporter::dumpNodes(file);

	return 0;
}