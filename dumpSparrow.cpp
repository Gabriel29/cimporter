#include <iostream>
#include <map>

#include "cimpAST.hpp"
#include "dumpSparrow.hpp"

namespace cimp
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
	std::stringstream ss;
	std::cout << "File: " << f->getFileName() << std::endl;
	std::cout << "------------------------" << std::endl << std::endl;
	
	/* Print all declarations from File */
	for(auto const& value: f->getDeclList())
	{
		auto nodeType = value->getNodeType();
		switch(nodeType)
		{
		
		case enumType:
			dumpEnum(reinterpret_cast<Enum*>(value->getNode()), ss);
			break;

		case typedefType:
			dumpTypedef(reinterpret_cast<Typedef*>(value->getNode()), ss);
			break;

		case structType:
			dumpStruct(reinterpret_cast<Struct*>(value->getNode()), ss);
			break;

		case funType:
			dumpFun(reinterpret_cast<Fun*>(value->getNode()), ss);
			break;

		default:
			break;
		}
	}

	std::cout << ss.str();
}


void dumpType(const Type* t, std::stringstream& ss)
{
	switch(t->getType())
	{
	case cimp_Int:
		ss << "Int";
		break;

	case cimp_Long:
		ss << "Long";
		break;

	case cimp_Char:
		ss << "Char";
		break;

	case cimp_Double:
		ss << "Double";
		break;

	case cimp_Pointer:
		ss << "Ptr(";
		dumpType(t->getChild(), ss);
		ss << ")";
		break;

	case cimp_Void:
		// This type is not present in Sparrow
		break;

	case cimp_CtArray:
		ss << "StaticArray(";
		dumpType(t->getChild(), ss);
		//ss << ", " << clang_getArraySize(t->getCXType());
		ss << ")";
		break;

	case cimp_IncArray:
		ss << "Ptr(";
		dumpType(t->getChild(), ss);
		ss << ")";
		break;

	// TODO - custom struct type
	// case CXType_Elaborated:
	// {
		// std::string name = getStructName(node->getCXType());
		// if(name.empty())
		// 	os << anonymousType;
		// else os << name;
	// 	break;
	// }
	default:
		break;
	}

}

void dumpEnum(Enum *e, std::stringstream& ss)
{
	ss << "using " << e->getName() << " = Int" << std::endl;
	for(auto const& value: e->getEnumList())
	{
		ss << "using " << value->getName() << " = " << value->getValue() << std::endl;
	}
}

void dumpStruct(Struct* s, std::stringstream& ss)
{
	ss << "using " << s->getName() << " = Int" << std::endl;
	for(auto const& value: s->getFieldList())
	{
		ss << "    " << value->getName() << " : ";
		dumpType(value->getType(), ss);
		ss << std::endl;
	}
}

void dumpTypedef(Typedef *t, std::stringstream& ss)
{
	ss << "using " << t->getName() << " = ";
	dumpType(t->getType(), ss);
	ss << std::endl;
}

void dumpFun(Fun* f, std::stringstream& ss)
{
	ss << "native[\"" << f->getName() << "\"]" << std::endl;
	ss << "fun " << f->getName();

	/* Print arguments */
	int i = 0;
	for(auto const& value: f->getParamList())
	{
		if(i == 0) ss << "(";
		if(i > 0) ss << ", ";

		dumpType(value->getType(), ss);
		i++;
	}
	if(i) ss << ")";

	// Parse return type
	std::stringstream retTypeStream;
	dumpType(f->getRetType(), retTypeStream);
	ss << " : " << retTypeStream.str();

	/* End of function */
	ss << std::endl;
}

// void dumpNodes(Object *obj)
// {
	
// 	else if(EnumNode *e = dynamic_cast<EnumNode*>(obj))
// 	{
// 		anonymousType = e->getName();
// 		std::cout << "using " << e->getName() << " = Int" << std::endl;
// 		dumpChildrenNodes(obj);
// 	}
	
// 	else if(EnumDeclNode *e = dynamic_cast<EnumDeclNode*>(obj))
// 	{
// 		std::cout << "using " << e->getName() << " = " << e->getValue() << std::endl;
// 	}
	
// 	else if(TypedefNode *t = dynamic_cast<TypedefNode*>(obj))
// 	{
// 		//std::ostringstream typedefStream;
	
// 		std::cout << "using " << t->getName() << " = ";
// 		dumpChildrenNodes(obj);
// 		std::cout << std::endl;

// 		//std::cout << typedefStream.std();
// 	}
	
// 	else if(FunctionDecl* f = dynamic_cast<FunctionDecl*>(obj))
// 	{
//  		std::ostringstream funcStream;
//  		std::ostringstream typeStream;
// 		TypeNode* retType;

// 		/* create function prototype */
// 		funcStream << "native[\"" << f->getName() << "\"]" << std::endl;
// 		funcStream << "fun " << f->getName();

// 		/* parse function parameters */
// 		for(std::vector<Object*>::size_type i = 1; i != obj->getChildren().size(); i++)
// 		{
// 			if(i == 1) funcStream <<" (";
// 			if(i > 2) funcStream << ", ";
// 			//dumpNodes(obj->getChildren()[i]);
// 			FunctionDeclParam* p = dynamic_cast<FunctionDeclParam*>(obj->getChildren()[i]);
// 			TypeNode* paramType = static_cast<TypeNode*>(p->getChildren()[0]);
// 			dumpType(paramType, typeStream);
// 			std::cout << "AICI -->" << typeStream.str();
// 			typeStream.clear();

// 			if(i+1 == obj->getChildren().size()) funcStream << ")";
// 		}

// 		/* get return type node */
// 		retType = static_cast<TypeNode*>(f->getChildren()[0]);
// 		dumpType(retType, typeStream);

// 		/* copy retType into funcStream */
// 		if(!typeStream.str().empty())
// 			funcStream << " : " << typeStream.str();
// 		funcStream << std::endl;

// 		/* print function */
// 		std::cout << funcStream.str();
// 	}
	
// 	else if(FunctionDeclParam* p = dynamic_cast<FunctionDeclParam*>(obj))
// 	{
// 		TypeNode* t = static_cast<TypeNode*>(p->getChildren()[0]);
// 		dumpType(t);
// 	}
	
// 	else if(StructNode* s = dynamic_cast<StructNode*>(obj))
// 	{
// 		anonymousType = s->getName();;
// 		std::cout << "datatype " << s->getName() << std::endl;
// 		dumpChildrenNodes(obj);
// 	}

// 	else if(FieldDecl* f = dynamic_cast<FieldDecl*>(obj))
// 	{
// 		std::cout << "    " << f->getName() << " : ";
// 		dumpChildrenNodes(obj);
// 		std::cout << std::endl;
// 	}
	
// 	else if(TypeNode* t = dynamic_cast<TypeNode*>(obj))
// 	{
// 		dumpType(t);
// 	}
// }

};