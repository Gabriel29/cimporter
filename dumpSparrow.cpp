#include <iostream>
#include "cimpAST.hpp"
#include "dumpSparrow.hpp"

namespace cimporter
{

void dumpFile(File* f)
{
	std::cout << "File: " << f->getFileName() << std::endl;
	std::cout << "------------------" << std::endl;

	for(std::vector<Object*>::iterator it = f->getChildren().begin(); it != f->getChildren().end(); ++it)
	{

	}
}

void dumpType(TypeNode* node)
{
	switch(node->getType().kind)
	{
	case CXType_Int:
	case CXType_UInt:
		std::cout << "Int";
		break;

	case CXType_Long:
	case CXType_LongLong:
	case CXType_ULong: 
	case CXType_ULongLong:
		break;

	case CXType_Char_S:
		break;

	case CXType_Double:
	case CXType_LongDouble: 
		break;

	case CXType_Pointer:
		break;

	case CXType_Void:
		break;

	case CXType_ConstantArray:
		break;

	case CXType_IncompleteArray:
		break;

	case CXType_Typedef:
		break;

	case CXType_Elaborated:
		break;

	default:
		break;
	}

}

void dumpChildrenNodes(Object *obj)
{
	for(std::vector<Object*>::size_type i = 0; i != obj->getChildren().size(); i++)
	{
		dumpNodes(obj->getChildren()[i]);
	}
}

void dumpNodes(Object *obj)
{
	if(File* f = dynamic_cast<File*>(obj))
	{
		std::cout << "File: " << f->getFileName() << std::endl;
		std::cout << "------------------" << std::endl;
		dumpChildrenNodes(obj);
	}
	else if(EnumNode *e = dynamic_cast<EnumNode*>(obj))
	{
		std::cout << "using " << e->getName() << " = Int" << std::endl;
		dumpChildrenNodes(obj);
	}
	else if(EnumDeclNode *e = dynamic_cast<EnumDeclNode*>(obj))
	{
		std::cout << "using" << e->getName() << " = " << e->getValue() << std::endl;
	}
	else if(TypedefNode *t = dynamic_cast<TypedefNode*>(obj))
	{
		std::cout << "using " << t->getName() << " = ";
		dumpChildrenNodes(obj);
		std::cout << std::endl;
	}
	else if(FunctionDecl* f = dynamic_cast<FunctionDecl*>(obj))
	{
		std::cout << "native[" << f->getName() << "]" << std::endl;
		std::cout << "fun " << f->getName() << std::endl;
		dumpChildrenNodes(obj);
	}
	else if(FunctionDeclParam *p = dynamic_cast<FunctionDeclParam*>(obj))
	{
		//std::cout << "-->" << p->getName() << std::endl;
	}
	else if(TypeNode* t = dynamic_cast<TypeNode*>(obj))
	{
		dumpType(t);
	}
}

/*
class dumpSparrow
{
public:
private:
};
*/

};