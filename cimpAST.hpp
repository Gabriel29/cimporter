#ifndef __CImpSprType__
#define __CImpSprType__

#include <vector>

/* C parser library */
#include <clang-c/Index.h>

namespace cimporter
{

class Object
{
protected:
	std::vector<Object*> children;
public:
	std::vector<Object*> getChildren()
	{
		return children;
	}

	virtual void addObject(Object *obj)
	{
		children.push_back(obj);
	}

};

class File : public Object
{
private:
	CXCursor cursor;
	CXIndex index;
	CXTranslationUnit tu;
	std::string fileName;
	
public:
	File(std::string file);
	~File();
	void parseFile();
	std::string getFileName()
	{
		return fileName;
	}
};

class TypeNode : public Object
{
public:
	TypeNode(CXType _type)
	{
		type = _type;
	}
	const CXType& getType() const {
		return type;
	}
private:
	CXType type;
};

class StructNode : public Object
{
public:
	StructNode(std::string name, TypeNode* type)
	{
		structName = name;
		structType = type;
	}
private:
	std::string structName;
	TypeNode *structType;	
};

class EnumNode : public Object
{
public:
	EnumNode(std::string name)
	{
		enumName = name;
	}
	const std::string& getName() const
	{
		return enumName;
	}
private:
	std::string enumName;
};

class EnumDeclNode : public Object
{
public:
	EnumDeclNode(std::string name, long value)
	{
		enumDeclName = name;
		enumDeclValue = value;
	}
	const std::string& getName() const
	{
		return enumDeclName;
	}
	const long long& getValue() const
	{
		return enumDeclValue;
	}
private:
	std::string enumDeclName;
	long long enumDeclValue;
};

class FieldDecl : public Object
{
public:
	FieldDecl(std::string name, long value)
	{
		fieldDeclName = name;
		//enumDeclValue = value;
	}
	const std::string& getName() const
	{
		return fieldDeclName;
	}
	// const long long& getValue() const
	// {
	// 	return enumDeclValue;
	// }
private:
	std::string fieldDeclName;
	//long long enumDeclValue;
};

class TypedefNode : public Object
{
public:
	TypedefNode(std::string typeName, Object* tNode)
	{
		name = typeName;
		this->addObject(tNode);
	}
	const std::string& getName() const {
		return name;
	}
private:
	std::string name;

};

/* children = [ return-type, arg0, arg1, ... ] */
class FunctionDecl : public Object
{
public:
	FunctionDecl(std::string name, Object* retType)
	{
		funcName = name;
		this->addObject(retType);
	}
	const std::string& getName() const {
		return funcName;
	}
private:
	std::string funcName;
};

class FunctionDeclParam : public Object
{
public:
	FunctionDeclParam(std::string name, Object* type)
	{
		paramName = name;
		this->addObject(type);
	}
	const std::string& getName() const {
		return paramName;
	}
	const Object* getType() const {
		return this->children[0];
	}
private:
	std::string paramName;
};

} /* namespace cimporter */

#endif