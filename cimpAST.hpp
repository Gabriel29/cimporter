#ifndef __CImpSprType__
#define __CImpSprType__

#include <vector>

/* C parser library */
#include <clang-c/Index.h>
#include "cimpMacroParser.hpp"

/*
 
The following classes describe how every node is represented in
AST that will afterwards be used in generating Sparrow code.

Object - basic object that all AST Nodes inherit

File
	[]

TypeNode
	CXType
	children [TypeNode]

FunctionDecl
	name
	children [retType, arg1, arg2, ...]

FunctionDeclParam
	[]

EnumNode
	enumName
	children [EnumDeclNode, EnumDeclNode, ...]

EnumDeclNode
	enumDeclName
	enumDeclValue

StructNode
	

FieldDecl
	[]

TypedefNode
	[]
*/

namespace cimp
{

CXChildVisitResult cursorVisitorPrep(CXCursor cursor, CXCursor parent, CXClientData clientData);
CXChildVisitResult cursorVisitorDecl(CXCursor cursor, CXCursor parent, CXClientData clientData);
CXChildVisitResult cursorVisitorEnums(CXCursor cursor, CXCursor parent, CXClientData clientData);
CXChildVisitResult cursorVisitorStruct(CXCursor cursor, CXCursor parent, CXClientData clientData);
CXChildVisitResult cursorVisitorFunc(CXCursor cursor, CXCursor parent, CXClientData clientData);

enum cimp_Type
{
	cimp_Bool = 0,
	cimp_Int, 
	cimp_Long,
	cimp_Char,
	cimp_Void,
	cimp_Float,
	cimp_Double,
	cimp_Pointer,
	cimp_CtArray,
	cimp_IncArray,
	cimp_Other,
};

enum cimp_nodeType
{
	enumType = 0,
	unionType,
	typedefType,	
	structType,
	funType,
};

class FunParam;

class Type
{
private:
	Type* child;
	//CXType cx_type;
	cimp_Type type;
	std::string data;
	std::vector<FunParam*> paramList;

public:
	Type(cimp_Type _type)
	{
		type = _type;
		child = NULL;
		data = "nothing";
	}

	Type(cimp_Type _type, Type* _child)
	{
		type = _type;
		child = _child;
		data = "nothing";
	}

	Type(cimp_Type _type, std::string _data)
	{
		type = _type;
		child = NULL;
		data = _data;
	}

	void addToList(FunParam* node)
	{
		paramList.push_back(node);
	}

	const cimp_Type getType() const {
		return type;
	}

	const Type* getChild() const {
		return child;
	}

	const std::string getData() const {
		return data;
	}
	void setData(std::string _data) {
		data = _data;
	}
	// TODO - Ptr, Array
};

class structField
{

private:
	std::string declName;
	Type* type;

public:
	structField(std::string name, Type* _type)
	{
		declName = name;
		type = _type;
	}
	const std::string& getName() const
	{
		return declName;
	}
	const Type* getType() const
	{
		return type;
	}
};

class Struct
{

private:
	std::string structName;
	CXType structType;
	std::vector<structField*> fieldList;

public:
	Struct(std::string name, CXType type)
	{
		structName = name;
		structType = type;
	}
	const std::string& getName() const
	{
		return structName;
	}
	const CXType getType() const
	{
		return structType;
	}
	const std::vector<structField*>& getFieldList() const
	{
		return fieldList;
	}
	void addToList(structField* node)
	{
		fieldList.push_back(node);
	}

};

class EnumDecl
{

private:
	std::string enumDeclName;
	long enumDeclValue;

public:
	EnumDecl(std::string name, long value)
	{
		enumDeclName = name;
		enumDeclValue = value;
	}
	const std::string& getName() const
	{
		return enumDeclName;
	}
	const long& getValue() const
	{
		return enumDeclValue;
	}
};

class Enum
{

private:
	std::string enumName;
	std::vector<EnumDecl*> enumList;

public:
	Enum(std::string name)
	{
		enumName = name;
	}
	const std::string& getName() const
	{
		return enumName;
	}
	const std::vector<EnumDecl*> getEnumList() const
	{
		return enumList;
	}
	void addToList(EnumDecl* node)
	{
		enumList.push_back(node);
	}
};

class FunParam
{

private:
	FunParam() { }
	std::string paramName;
	Type *type;

public:
	FunParam(std::string name, Type* _type)
	{
		paramName = name;
		type = _type;
	}
	const std::string& getName() const {
		return paramName;
	}
	const Type* getType() const {
		return type;
	}
};

class FunPtr
{

private:
	FunPtr() { }
	std::string funcName;
	Type* retType;
	std::vector<FunParam*> paramList;

public:
	FunPtr(std::string name, Type* _retType)
	{
		funcName = name;
		retType = _retType;
	}
	const std::string& getName() const {
		return funcName;
	}
	const std::vector<FunParam*> getParamList() const {
		return paramList;
	}
	const Type* getRetType() const {
		return retType;
	}

	void addToList(FunParam* node)
	{
		paramList.push_back(node);
	}
};

class Fun
{

private:
	Fun() { }
	std::string funcName;
	Type* retType;
	std::vector<FunParam*> paramList;

public:
	Fun(std::string name, Type* _retType)
	{
		funcName = name;
		retType = _retType;
	}
	const std::string& getName() const {
		return funcName;
	}
	const std::vector<FunParam*> getParamList() const {
		return paramList;
	}
	const Type* getRetType() const {
		return retType;
	}

	void addToList(FunParam* node)
	{
		paramList.push_back(node);
	}
};

class Typedef
{

private:
	std::string name;
	bool _isFunPtr;
	Type *type;
	std::vector<FunParam*> paramList;

	Typedef() { }

public:
	Typedef(std::string typeName, Type* _type)
	{
		name = typeName;
		type = _type;
		_isFunPtr = false;
	}
	void setFunPtrTrue() {
		_isFunPtr = true;
	}
	bool isFunPtr() {
		return _isFunPtr;
	}
	const std::string& getName() const {
		return name;
	}
	const Type* getType() const {
		return type;
	}
	void addToList(FunParam* decl) {
		paramList.push_back(decl);
	}
	const std::vector<FunParam*>& getParamList() const
	{
		return paramList;
	}
};

class Decl
{

private:
	void *node;
	enum cimp_nodeType nodeType;
	Decl() { }

public:
	Decl(Enum* e)
	{
		node = (void*)e;
		nodeType = enumType;
	}
	Decl(Typedef* t)
	{
		node = (void*)t;
		nodeType = typedefType;
	}
	Decl(Struct* s)
	{
		node = (void*)s;
		nodeType = structType;
	}
	Decl(Fun* f)
	{
		node = (void*)f;
		nodeType = funType;
	}

	cimp_nodeType getNodeType() const
	{
		return nodeType;
	}

	void* getNode() const
	{
		return node;
	}
};

class File
{
private:
	CXCursor cursor;
	CXIndex index;
	CXTranslationUnit tu;
	std::string fileName;
	std::vector<Decl*> declList;
	std::vector<Macro*> macroList;
	File() { }
	
public:
	File(std::string file);
	~File();
	void parseFile();
	std::string getFileName()
	{
		return fileName;
	}
	const CXTranslationUnit& getTranslationUnit() const
	{
		return tu;
	}

	const std::vector<Decl*>& getDeclList() const
	{
		return declList;
	}

	const std::vector<Macro*>& getMacroList() const
	{
		return macroList;
	}

	void addToList(Decl* decl) {
		declList.push_back(decl);
	}
	void addToMacroList(Macro* node) {
		macroList.push_back(node);
	}
};

} /* namespace cimporter */

#endif