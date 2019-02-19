#include <iostream>
#include <string>
#include <vector>

#include "cimpAST.hpp"
#include "utils.h"

#include <stdio.h>


/* cimporter namespace */
namespace cimp 
{

std::string lastAnonymousName;
std::string currentFile;
unsigned int currentVal;

Type* parseType(CXType cx_type)
{
	Type *type, *child;
	
	switch(cx_type.kind)
	{
	case CXType_Bool:
		type = new Type(cimp_Bool);
		break;

	case CXType_Int:
		type = new Type(cimp_Int);
		break;

	case CXType_Long:
	case CXType_LongLong:
		type = new Type(cimp_Long);
		break;

	case CXType_Float:
		type = new Type(cimp_Float);
		break;

	case CXType_Double:
		type = new Type(cimp_Double);
		break;

	case CXType_Char_S:
		type = new Type(cimp_Char);
		break;

	case CXType_Pointer:
		child = parseType(clang_getPointeeType(cx_type));
		type = new Type(cimp_Pointer, child);
		break;

	case CXType_ConstantArray:
		child = parseType(clang_getArrayElementType(cx_type));
		type = new Type(cimp_CtArray, child);
		break;

	case CXType_IncompleteArray:
		child = parseType(clang_getArrayElementType(cx_type));
		type = new Type(cimp_IncArray, child);
		break;

	case CXType_Void:
		type = new Type(cimp_Void);
		break;

	default:
		break;
	}

	return type;
}

Typedef* parseTypedef(CXCursor cursor)
{
	std::string name = getCursorName(cursor);
	CXType cx_type = clang_getTypedefDeclUnderlyingType(cursor);
	Type *type = parseType(cx_type);
	Typedef *t = new Typedef(name, type);

	return t;
}

EnumDecl* parseEnumDecl(const CXCursor& cursor)
{
	std::string enumName = getCursorName(cursor);
	long enumValue = clang_getEnumConstantDeclValue(cursor);
	EnumDecl* e = new EnumDecl(enumName, enumValue);

	return e;
}

Enum* parseEnum(const CXCursor& cursor)
{
	std::string name = getCursorName(cursor);
	if(name.empty()) {
		lastAnonymousName = currentFile + std::to_string(currentVal);
		name = currentFile + std::to_string(currentVal);
		currentVal++;
	}
	Enum *e = new Enum(name);
	clang_visitChildren(cursor, cursorVisitorEnums, e);

	return e;
}

Struct* parseStruct(const CXCursor& cursor)
{
	std::string name = getCursorName(cursor);

	if(name.empty()) {
		lastAnonymousName = currentFile + std::to_string(currentVal);
		name = currentFile + std::to_string(currentVal);
		currentVal++;
	}

	CXType type = clang_getCursorType(cursor);
	Struct* s = new Struct(name, type);
	clang_visitChildren(cursor, cursorVisitorStruct, s);

	return s;
}

structField* parseStructField(CXCursor cursor)
{
	std::string name = getCursorName(cursor);
	Type* type = parseType(clang_getCursorType(cursor));
	
	return new structField(name, type);
}

Fun* parseFun(CXCursor cursor)
{
	Fun* func = NULL;
	std::string name = getCursorName(cursor);
	Type* retType = parseType(clang_getResultType(clang_getCursorType(cursor)));

	/* Create function node */
	func = new Fun(name, retType);

	/* Parse args if they are available */
	if(clang_Cursor_getNumArguments(cursor) > 0)
	{
		clang_visitChildren(cursor, cursorVisitorFunc, func);
	}

	return func;
}

FunParam* parseFunParam(CXCursor cursor)
{
	FunParam* param = NULL;
	param = new FunParam("nume", parseType(clang_getCursorType(cursor)));
	
	return param;
}

CXChildVisitResult cursorVisitorFunc(CXCursor cursor, CXCursor parent, CXClientData clientData)
{
	CXSourceLocation location = clang_getCursorLocation(cursor);
	if( clang_Location_isFromMainFile( location ) == 0 )
	{
		return CXChildVisit_Continue;
	}

	Fun* f = reinterpret_cast<Fun*>(clientData);
	CXCursorKind cursorKind = clang_getCursorKind(cursor);

	if (cursorKind == CXCursor_ParmDecl) {
		f->addToList(parseFunParam(cursor));
	}

	return CXChildVisit_Continue;
}

CXChildVisitResult cursorVisitorStruct(CXCursor cursor, CXCursor parent, CXClientData clientData)
{
	CXSourceLocation location = clang_getCursorLocation(cursor);
	if( clang_Location_isFromMainFile( location ) == 0 )
	{
		return CXChildVisit_Continue;
	}

	Struct* s = reinterpret_cast<Struct*>(clientData);
	CXCursorKind cursorKind = clang_getCursorKind(cursor);

	if (cursorKind ==  CXCursor_FieldDecl) {
		s->addToList(parseStructField(cursor));
	}

	return CXChildVisit_Continue;
}

CXChildVisitResult cursorVisitorEnums(CXCursor cursor, CXCursor parent, CXClientData clientData)
{
	CXSourceLocation location = clang_getCursorLocation(cursor);
	if( clang_Location_isFromMainFile( location ) == 0 )
	{
		return CXChildVisit_Continue;
	}

	Enum* e = reinterpret_cast<Enum*>(clientData);
	CXCursorKind cursorKind = clang_getCursorKind(cursor);

	if (cursorKind ==  CXCursor_EnumConstantDecl) {
		e->addToList(parseEnumDecl(cursor));
	}

	return CXChildVisit_Continue;
}

CXChildVisitResult cursorVisitorDecl(CXCursor cursor, CXCursor parent, CXClientData clientData)
{
	CXSourceLocation location = clang_getCursorLocation(cursor);
	if( clang_Location_isFromMainFile( location ) == 0 )
	{
		return CXChildVisit_Continue;
	}

	CXCursorKind cursorKind = clang_getCursorKind(cursor);

	switch(cursorKind)
	{
	case CXCursor_TypedefDecl:
	{
		//obj->addObject(parseTypedef(cursor));
		File* f = reinterpret_cast<File*>(clientData);
		f->addToList(new Decl(parseTypedef(cursor)));
		break;
	}

	case CXCursor_EnumDecl: 
	{
		File* f = reinterpret_cast<File*>(clientData);
		f->addToList(new Decl(parseEnum(cursor)));
		break;
	}

	case CXCursor_StructDecl:
	{
		File* f = reinterpret_cast<File*>(clientData);
		f->addToList(new Decl(parseStruct(cursor)));
		break;
	}

	// case CXCursor_FieldDecl:
	// {
	// 	Struct* s = reinterpret_cast<Struct*>(clientData);
	// 	s->addToList(parseStructField(cursor));
	//  	break;
	// }

	case CXCursor_UnionDecl:
	{
		File* f = reinterpret_cast<File*>(clientData);
		//f->addToList(new Decl(parseUnion(cursor)));
		break;
	}

	case CXCursor_FunctionDecl:
	{
		// obj->addObject(parseFunctionDecl(cursor));
		File* f = reinterpret_cast<File*>(clientData);
		f->addToList(new Decl(parseFun(cursor)));
		break; 
	}

	//case CXCursor_VarDecl:
	//	break;

	// case CXCursor_ParmDecl:
	// 	// obj->addObject(parseParmDecl(cursor));
	// 	break;

	// case CXCursor_MacroDefinition:
	// 	break;

	// case CXCursor_MacroExpansion:
	// 	break;

	// /* Parse included files */
	// case CXCursor_InclusionDirective:
	// 	break;

	default:
		break;
	}

	return CXChildVisit_Continue;
}

std::string getMacroType(std::string type)
{
	std::string sprType;

	if(type == "int")
		sprType = "Int";
	else if(type == "long")
		sprType = "Int";
	else if(type == "unsigned")
		sprType = "Int";
	else if(type == "float")
		sprType = "Double";
	else if(type == "double")
		sprType = "Double";
	else if(type == "char")
		sprType = "Char";
	else sprType = type;

	return sprType;
}

Macro* parseMacroDefiniton(CXCursor cursor, File* file)
{
	
	CXTranslationUnit TU = (CXTranslationUnit)file->getTranslationUnit();
	CXSourceRange range = clang_getCursorExtent(cursor);

	std::string name, macro;
	unsigned  num_tokens;
	CXToken*  tokens;
	CXCursor* cursors = 0;
	clang_tokenize(TU, range, &tokens, &num_tokens);

	cursors = (CXCursor *)malloc(num_tokens * sizeof(CXCursor));
	clang_annotateTokens(TU, tokens, num_tokens, cursors);

	for (unsigned i = 0; i != num_tokens; ++i) {
		CXString str = clang_getTokenSpelling(TU, tokens[i]);
		
		if(i == 0)
			name = clang_getCString(str);
		else if(i == 1) 
			macro = getMacroType(clang_getCString(str));

		clang_disposeString(str);
	}

	free(cursors);
	clang_disposeTokens(TU, tokens, num_tokens);

	return new Macro(name, macro);
}

CXChildVisitResult cursorVisitorPrep(CXCursor cursor, CXCursor parent, CXClientData clientData)
{
	CXSourceLocation location = clang_getCursorLocation(cursor);
	if( clang_Location_isFromMainFile( location ) == 0 )
	{
		return CXChildVisit_Continue;
	}

	CXCursorKind cursorKind = clang_getCursorKind(cursor);

	switch(cursorKind)
	{

	case CXCursor_MacroDefinition:
	{
		File* f = reinterpret_cast<File*>(clientData);
		f->addToMacroList(parseMacroDefiniton(cursor, (File*)clientData));
		break;
	}

	case CXCursor_MacroExpansion:
	{
		break;
	}

	/* Parse included files */
	case CXCursor_InclusionDirective:
	{
		break;
	}

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
	/* Parse prep */
	clang_visitChildren(cursor, cursorVisitorPrep, this);

	/* Parse decl */
	clang_visitChildren(cursor, cursorVisitorDecl, this);
}

} /* cimporter namespace */