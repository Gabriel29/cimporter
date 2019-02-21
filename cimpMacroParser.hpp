#ifndef __cimpMacroParser__
#define __cimpMacroParser__

#include <vector>
#include <string>

class Macro
{
private:
	std::string name;
	std::string macro;

public:
	Macro(std::string _name, std::string _macro)
	{
		name = _name;
		macro = _macro;
	}
	const std::string& getName() const
	{
		return name;
	}
	const std::string& getExpansion() const
	{
		return macro;
	}

	//static Macro* parseMacroDefiniton(CXCursor cursor, File* file);
};

class MacroParser
{
	enum tkType
	{
		tkNUMBER,
		tkHEXNUMBER,
		tkSTRING,
		tkLPARAN,
		tkRPARAN,
		tkCOMMA,
	};

private:
	std::vector<std::string>* tokens;
	unsigned int pos;

	bool isLast();
	MacroParser::tkType getType(unsigned int);
public:
	MacroParser(std::vector<std::string>* _tokens)
	{
		tokens = _tokens;
		pos = 0;
	}

	bool isNext(MacroParser::tkType);
	bool isNext2(MacroParser::tkType);
	bool isType(MacroParser::tkType);

	Macro* parseMacro();
};

#endif /* __cimpMacroParser__ */