#include <iostream>
#include <string>
#include <vector>
#include "cimpAST.hpp"

#include <stdio.h>


/* cimporter namespace */
namespace cimporter 
{

std::string lastAnonymousName;
std::string currentFile;
unsigned int currentVal;

std::string removeFileExtension(const std::string& file) {
    size_t dot = file.find_last_of(".");
    if (dot == std::string::npos) 
    	return file;
    return file.substr(0, dot); 
}

CXChildVisitResult cursorVisitor(CXCursor cursor, CXCursor parent, CXClientData clientData);

std::string getCursorName(CXCursor cursor)
{
	CXString cursorSpelling = clang_getCursorSpelling(cursor);
	std::string cursorName(clang_getCString(cursorSpelling));
	clang_disposeString(cursorSpelling);

	return cursorName;
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

TypedefNode* parseTypedef(CXCursor cursor)
{
	std::string name = getCursorName(cursor);
	CXType type = clang_getTypedefDeclUnderlyingType(cursor);
	TypeNode *Type = parseType(type);
	TypedefNode *Typedef = new TypedefNode(name, Type);

	return Typedef;
}

EnumNode* parseEnum(CXCursor cursor)
{
	std::string name = getCursorName(cursor);
	if(name.empty()) {
		lastAnonymousName = currentFile + std::to_string(currentVal);
		name = currentFile + std::to_string(currentVal);
		currentVal++;
	}
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

StructNode* parseStructDecl(const CXCursor &cursor)
{
	std::string name = getCursorName(cursor);
	if(name.empty()) {
		lastAnonymousName = currentFile + std::to_string(currentVal);
		name = currentFile + std::to_string(currentVal);
		currentVal++;
	}
	
	TypeNode* type = new TypeNode(clang_getCursorType(cursor));
	StructNode* s = new StructNode(name, type);
	clang_visitChildren(cursor, cursorVisitor, s);

	return s;
}

FieldDecl* parseFieldDecl(CXCursor cursor)
{
	std::string name = getCursorName(cursor);
	TypeNode* type = parseType(clang_getCursorType(cursor));
	
	return new FieldDecl(name, type);
}

FunctionDecl* parseFunctionDecl(CXCursor cursor)
{
	FunctionDecl* func = NULL;
	std::string name = getCursorName(cursor);
	TypeNode* retType = parseType(clang_getResultType(clang_getCursorType(cursor)));

	/* Create function node */
	func = new FunctionDecl(name, retType);

	/* Parse args if they are available */
	if(clang_Cursor_getNumArguments(cursor) > 0)
	{
		clang_visitChildren(cursor, cursorVisitor, func);
	}

	return func;
}

FunctionDeclParam* parseParmDecl(CXCursor cursor)
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
		obj->addObject(parseParmDecl(cursor));
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
	currentFile = removeFileExtension(file);
	currentVal = 0;

	index = clang_createIndex(1, 1);
	tu = clang_parseTranslationUnit(index, fileName.c_str(), NULL, 0, NULL,
                              0, CXTranslationUnit_DetailedPreprocessingRecord);

	if(tu == NULL) {
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
