#include "utils.h"
#include <iostream>

namespace cimp
{

std::string getFilePath(const std::string& filePath)
{
	std::string file = "";

 	// Get file Path
	size_t fileNamePos = filePath.find_last_of("/");
	if(fileNamePos != std::string::npos)
		file = file.substr(fileNamePos + 1);

	return filePath.substr(0, fileNamePos);
}

std::string removeFileExtension(const std::string& fileCt) {
	std::string file = fileCt;

 	// Get only file Name
	size_t fileName = file.find_last_of("/");
	if(fileName != std::string::npos)
		file = file.substr(fileName + 1);

	// Remove file extension
    size_t dot = file.find_last_of(".");
    if (dot == std::string::npos) 
    	return file;
    
	return file.substr(0, dot); 
}

std::string getCursorName(const CXCursor& cursor)
{
	CXString cursorSpelling = clang_getCursorSpelling(cursor);
	std::string cursorName(clang_getCString(cursorSpelling));
	clang_disposeString(cursorSpelling);

	return cursorName;
} 

std::string getStructName(CXType type)
{
	CXCursor cursor = clang_getTypeDeclaration(type);
	CXString cursorSpelling = clang_getCursorSpelling (cursor);
	std::string sprType = std::string(clang_getCString(cursorSpelling));
	clang_disposeString(cursorSpelling);

	return sprType;
}

} /* namespace cimp */
