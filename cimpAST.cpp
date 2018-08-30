#include <iostream>
#include <string>
#include <vector>
#include "cimpAST.hpp"

#include <stdio.h>


/* cimporter namespace */
namespace cimporter 
{
	CXChildVisitResult cursorVisitor(CXCursor cursor, CXCursor parent, CXClientData clientData);

	std::string getCursorName(CXCursor cursor)
	{
		CXString cursorSpelling = clang_getCursorSpelling(cursor);
		std::string cursorName(clang_getCString(cursorSpelling));
		clang_disposeString(cursorSpelling);

		return cursorName;
	}

	TypedefNode* parseTypedef(CXCursor cursor)
	{
		std::string name = getCursorName(cursor);
		CXType type = clang_getTypedefDeclUnderlyingType(cursor);
		TypeNode *Type = new TypeNode(type);
		TypedefNode *Typedef = new TypedefNode(name, Type);

		return Typedef;
	}

	EnumNode* parseEnum(CXCursor cursor)
	{
		std::string name = getCursorName(cursor);
		EnumNode *e = new EnumNode(name);
		clang_visitChildren(cursor, cursorVisitor, e);

		return e;
	}

	EnumDeclNode* parseEnumDecl(CXCursor cursor)
	{
		std::string enumName = getCursorName(cursor);
		long enumValue = clang_getEnumConstantDeclValue(cursor);
		EnumDeclNode* e = new EnumDeclNode(enumName, enumValue);
		return e;
	}

	StructNode* parseStructDecl(CXCursor cursor)
	{
		std::string name = getCursorName(cursor);
		TypeNode* type = new TypeNode(clang_getCursorType(cursor));
		StructNode* s = new StructNode(name, type);
		clang_visitChildren(cursor, cursorVisitor, s);

		return s;
	}

	FieldDecl* parseFieldDecl(CXCursor cursor)
	{
		std::string name = getCursorName(cursor);
		return new FieldDecl(name, 0); // TODO
	}

	TypeNode* parseType(CXType type)
	{
		TypeNode *node = new TypeNode(type);
		
		switch(type.kind)
		{
		case CXType_Pointer:
			node->addObject(parseType(clang_getPointeeType(type)));
			break;
		case CXType_ConstantArray:
		 	node->addObject(parseType(clang_getArrayElementType(type)));
			break;
		case CXType_IncompleteArray:
		 	node->addObject(parseType(clang_getArrayElementType(type)));
			break;
		}

		return node;
	}

	FunctionDecl* parseFunctionDecl(CXCursor cursor)
	{
		FunctionDecl* func = NULL;
		std::string name = getCursorName(cursor);
		TypeNode* retType = parseType(clang_getResultType(clang_getCursorType(cursor)));

		func = new FunctionDecl(name, retType);

		if(clang_Cursor_getNumArguments(cursor) > 0)
		{
			clang_visitChildren(cursor, cursorVisitor, func);
		}

		return func;
	}

	FunctionDeclParam* parseParnDecl(CXCursor cursor)
	{
		FunctionDeclParam* param = NULL;
		param = new FunctionDeclParam("nume", parseType(clang_getCursorType(cursor)));
		
		return param;
	}

	CXChildVisitResult cursorVisitor(CXCursor cursor, CXCursor parent, CXClientData clientData)
	{
		CXSourceLocation location = clang_getCursorLocation(cursor);
		if( clang_Location_isFromMainFile( location ) == 0 )
		{
			return CXChildVisit_Continue;
		}

		Object* obj = static_cast<Object*>(clientData);
		CXCursorKind cursorKind = clang_getCursorKind(cursor);

		switch(cursorKind)
		{
		case CXCursor_TypedefDecl:
			obj->addObject(parseTypedef(cursor));
			break;

		case CXCursor_EnumDecl:
			obj->addObject(parseEnum(cursor));
			break;

		case CXCursor_StructDecl:
			obj->addObject(parseStructDecl(cursor));
			break;

		case CXCursor_FieldDecl:
			obj->addObject(parseFieldDecl(cursor));
		 	break;

		case CXCursor_UnionDecl:
			break;

		case CXCursor_FunctionDecl:
			obj->addObject(parseFunctionDecl(cursor));
			break; 

		case CXCursor_VarDecl:
			break;

		case CXCursor_ParmDecl:
			obj->addObject(parseParnDecl(cursor));
			break;

		case CXCursor_EnumConstantDecl: 
			obj->addObject(parseEnumDecl(cursor));
			break;

		case CXCursor_MacroDefinition:
			break;

		case CXCursor_MacroExpansion:
			break;

		default:
			break;
		}

		return CXChildVisit_Continue;
	}

	File::File(std::string file)
	{
		fileName = file;
		index = clang_createIndex(0, 0);
		tu = clang_parseTranslationUnit(index, fileName.c_str(), NULL, 0, NULL,
                                  0, CXTranslationUnit_DetailedPreprocessingRecord);
	
		if( !tu) {
			std::cout << "parseTranslationUnit failed to initiliaze" << std::endl;
			//return -1;
		}

		cursor = clang_getTranslationUnitCursor(tu);
	}

	/* File destructor */
	File::~File()
	{
		clang_disposeTranslationUnit(tu);
		clang_disposeIndex(index);
	}

	void File::parseFile()
	{
		clang_visitChildren(cursor, cursorVisitor, this);
	}

} /* cimporter namespace */
