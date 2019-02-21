#ifndef __cimpMacroParser__
#define __cimpMacroParser__

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
private:
	std::vector<std::string>* tokens;
public:
	MacroParser(std::vector<std::string>* _tokens)
	{
		tokens = _tokens;
	}
	
	static bool isNext();
	static bool isNext2();
	static bool accept();
};

#endif /* __cimpMacroParser__ */