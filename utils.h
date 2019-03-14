#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <clang-c/Index.h>

namespace cimp
{

CXChildVisitResult cursorFuncProto(CXCursor cursor, CXCursor parent, CXClientData clientData);

std::string getCursorName(const CXCursor& cursor);
std::string removeFileExtension(const std::string& file);
std::string getStructName(CXType type);
std::string getFunctionProto(CXType type);

} /* namespace cimp */

#endif /* UTILS_H */