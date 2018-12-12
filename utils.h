#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <clang-c/Index.h>

namespace cimp
{

std::string getCursorName(const CXCursor& cursor);
std::string removeFileExtension(const std::string& file);

} /* namespace cimp */

#endif /* UTILS_H */