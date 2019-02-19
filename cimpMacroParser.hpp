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

#endif /* __cimpMacroParser__ */