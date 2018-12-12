#ifndef __DUMP_SPARROW__
#define __DUMP_SPARROW__

#include <sstream>
#include "cimpAST.hpp"

namespace cimp
{

void dumpFile(File* f);
void dumpEnum(Enum *e, std::stringstream& ss);
void dumpTypedef(Typedef* t, std::stringstream& ss);
void dumpStruct(Struct* s, std::stringstream& ss);
void dumpFun(Fun *f, std::stringstream& ss);

};

#endif /* __DUMP_SPARROW__ */