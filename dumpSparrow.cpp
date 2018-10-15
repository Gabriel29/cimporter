#include <iostream>
#include <sstream>
#include <map>

#include "cimpAST.hpp"
#include "dumpSparrow.hpp"

namespace cimporter
{

std::string anonymousType;

std::string getStructName(CXType type)
{
	auto cursor = clang_getTypeDeclaration(type);
	auto cursorSpelling = clang_getCursorSpelling (cursor);
	std::string sprType = std::string(clang_getCString(cursorSpelling));
	clang_disposeString(cursorSpelling);

	return sprType;
}

void dumpFile(File* f)
{
	// std::cout << "File: " << f->getFileName() << std::endl;
	// std::cout << "------------------" << std::endl;
	for(std::vector<Object*>::iterator it = f->getChildren().begin(); it != f->getChildren().end(); ++it)
	{

	}
}

void dumpType(TypeNode* node)
{
	switch(node->getCXType().kind)
	{
	case CXType_Int:
	case CXType_UInt:
		std::cout << "Int";
		break;

	case CXType_Long:
	case CXType_LongLong:
	case CXType_ULong: 
	case CXType_ULongLong:
		std::cout << "Long";
		break;

	case CXType_Char_S:
		std::cout << "Char";
		break;

	case CXType_Double:
	case CXType_LongDouble:
		std::cout << "Double";
		break;

	case CXType_Pointer:
		std::cout << "Ptr(";
		dumpNodes(node->getChildren()[0]);
		std::cout << ")";
		break;

	case CXType_Void:
		// This type is not present in Sparrow
		break;

	case CXType_ConstantArray:
		std::cout << "StaticArray(";
		dumpNodes(node->getChildren()[0]);
		std::cout << ", " << clang_getArraySize(node->getCXType());
		std::cout << ")";
		break;

	case CXType_IncompleteArray:
		std::cout << "Ptr(";
		dumpNodes(node->getChildren()[0]);
		std::cout << ")";
		break;

	case CXType_Typedef:
		break;

	case CXType_Elaborated:
	{
		std::string name = getStructName(node->getCXType());
		if(name.empty())
			std::cout << anonymousType;
		else std::cout << name;
		break;
	}
	default:
		break;
	}

}

void dumpType(TypeNode* node, std::ostringstream& os)
{
	switch(node->getCXType().kind)
	{
	case CXType_Int:
	case CXType_UInt:
		os << "Int";
		break;

	case CXType_Long:
	case CXType_LongLong:
	case CXType_ULong: 
	case CXType_ULongLong:
		os << "Long";
		break;

	case CXType_Char_S:
		os << "Char";
		break;

	case CXType_Double:
	case CXType_LongDouble:
		os << "Double";
		break;

	case CXType_Pointer:
		os << "Ptr(";
		dumpNodes(node->getChildren()[0]);
		os << ")";
		break;

	case CXType_Void:
		// This type is not present in Sparrow
		break;

	case CXType_ConstantArray:
		os << "StaticArray(";
		dumpNodes(node->getChildren()[0]);
		os << ", " << clang_getArraySize(node->getCXType());
		os << ")";
		break;

	case CXType_IncompleteArray:
		os << "Ptr(";
		dumpNodes(node->getChildren()[0]);
		os << ")";
		break;

	case CXType_Typedef:
		break;

	case CXType_Elaborated:
	{
		std::string name = getStructName(node->getCXType());
		if(name.empty())
			os << anonymousType;
		else os << name;
		break;
	}
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
		// std::cout << "File: " << f->getFileName() << std::endl;
		// std::cout << "------------------" << std::endl;
		dumpChildrenNodes(obj);
	}
	
	else if(EnumNode *e = dynamic_cast<EnumNode*>(obj))
	{
		anonymousType = e->getName();
		std::cout << "using " << e->getName() << " = Int" << std::endl;
		dumpChildrenNodes(obj);
	}
	
	else if(EnumDeclNode *e = dynamic_cast<EnumDeclNode*>(obj))
	{
		std::cout << "using " << e->getName() << " = " << e->getValue() << std::endl;
	}
	
	else if(TypedefNode *t = dynamic_cast<TypedefNode*>(obj))
	{
		//std::ostringstream typedefStream;
	
		std::cout << "using " << t->getName() << " = ";
		dumpChildrenNodes(obj);
		std::cout << std::endl;

		//std::cout << typedefStream.std();
	}
	
	else if(FunctionDecl* f = dynamic_cast<FunctionDecl*>(obj))
	{
 		std::ostringstream funcStream;
 		std::ostringstream typeStream;
		TypeNode* retType;

		/* create function prototype */
		funcStream << "native[\"" << f->getName() << "\"]" << std::endl;
		funcStream << "fun " << f->getName();

		/* parse function parameters */
		for(std::vector<Object*>::size_type i = 1; i != obj->getChildren().size(); i++)
		{
			if(i == 1) funcStream <<" (";
			if(i > 2) funcStream << ", ";
			dumpNodes(obj->getChildren()[i]);
			if(i+1 == obj->getChildren().size()) funcStream << ")";
		}

		/* get return type node */
		retType = static_cast<TypeNode*>(f->getChildren()[0]);
		dumpType(retType, typeStream);

		/* copy retType into funcStream */
		if(!typeStream.str().empty())
			funcStream << " : " << typeStream.str();
		funcStream << std::endl;

		/* print function */
		std::cout << funcStream.str();
	}
	
	else if(FunctionDeclParam* p = dynamic_cast<FunctionDeclParam*>(obj))
	{
		TypeNode* t = static_cast<TypeNode*>(p->getChildren()[0]);
		dumpType(t);
	}
	
	else if(StructNode* s = dynamic_cast<StructNode*>(obj))
	{
		anonymousType = s->getName();;
		std::cout << "datatype " << s->getName() << std::endl;
		dumpChildrenNodes(obj);
	}

	else if(FieldDecl* f = dynamic_cast<FieldDecl*>(obj))
	{
		std::cout << "    " << f->getName() << " : ";
		dumpChildrenNodes(obj);
		std::cout << std::endl;
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