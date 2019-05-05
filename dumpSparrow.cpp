#include <iostream>
#include <map>

#include "cimpAST.hpp"
#include "dumpSparrow.hpp"
#include "utils.h"

namespace cimp
{

std::string anonymousType;

void dumpFile(File* f)
{
	std::string file = f->getFileName();
	std::stringstream ss;

	std::cout << "import cimporter.cimp" << std::endl;
	std::cout << std::endl;
	
	/* Print all macros from File */
	for(auto const& value: f->getMacroList())
	{
		if(value->isValid())
			std::cout << "using " << value->getName() << " = " << value->getExpansion() << std::endl;
	}

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
		
		case unionType:
			dumpUnion(reinterpret_cast<Union*>(value->getNode()), ss);

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

	case cimp_ULong:
		ss << "ULong";
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
		if(t->getChild()->getType() == cimp_Void)
			ss << "Byte";

		dumpType(t->getChild(), ss);
		ss << ")";
		break;

	case cimp_Void:
		// This type is not present in Sparrow
		//ss << "Byte";
		break;

	case cimp_CtArray:
		ss << "StaticArray(";
		dumpType(t->getChild(), ss);
		ss << ", " << t->getData();
		ss << ")";
		break;

	case cimp_IncArray:
		ss << "Ptr(";

		dumpType(t->getChild(), ss);
		ss << ")";
		break;

	case cimp_Other:
	{
		std::string name = t->getData();
		
		if(name.empty()) {
			ss << anonymousType;
		}
		else ss << name;
		break;
	}

	default:
		break;
	}

}

void dumpEnum(Enum *e, std::stringstream& ss)
{
	anonymousType = e->getName();
	ss << "using " << e->getName() << " = Int" << std::endl;
	for(auto const& value: e->getEnumList())
	{
		ss << "using " << value->getName() << " = " << value->getValue() << std::endl;
	}
}

void dumpUnion(Union *u, std::stringstream& ss)
{
	bool first = true;
	anonymousType = u->getName();
	ss << "using " << u->getName() << " = Union(";
	for(auto const& value: u->getFieldList())
	{
		if(!first) ss << ", ";
		dumpType(value->getType(), ss);
		first = false;
	}
	ss << ")" << std::endl;
}

void dumpStruct(Struct* s, std::stringstream& ss)
{
	anonymousType = s->getName();
	ss << "datatype " << s->getName() << std::endl;
	for(auto const& value: s->getFieldList())
	{
		ss << "    " << value->getName() << " : ";
		dumpType(value->getType(), ss);
		ss << std::endl;
	}
}

void dumpTypedef(Typedef *t, std::stringstream& ss)
{
	auto type = t->getType();
	ss << "using " << t->getName() << " = ";
	if(t->isFunPtr() == false)
		dumpType(type, ss);
	else {
		ss << "FunctionPtr(";
		dumpType(type, ss);

		/* Print arguments */
		int i = 0;
		for(auto const& value: t->getParamList())
		{
			if(i == 0) ss << ", ";
			if(i > 0) ss << ", ";

			dumpType(value->getType(), ss);
			i++;
		}

		ss << ")";

	}
	ss << std::endl;
}

void dumpFun(Fun* f, std::stringstream& ss)
{
	ss << "[native(\"" << f->getName() << "\")]" << std::endl;
	ss << "fun " << f->getName();

	/* Print arguments */
	int i = 0;
	for(auto const& value: f->getParamList())
	{
		std::string parVarName = "var" + std::to_string(i);
		if(i == 0) ss << "(";
		if(i > 0) ss << ", ";

		ss << parVarName << ": ";
		dumpType(value->getType(), ss);
		i++;
	}
	if(i) ss << ")";

	// Parse return type
	std::stringstream retTypeStream;
	dumpType(f->getRetType(), retTypeStream);
	if(!retTypeStream.str().empty())
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