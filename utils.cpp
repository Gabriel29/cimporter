#include "utils.h"

namespace cimp
{

std::string removeFileExtension(const std::string& file) {
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

} /* namespace cimp */
