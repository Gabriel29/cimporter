#include <iostream>

#include "cimpMacroParser.hpp"

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

MacroParser::tkType MacroParser::getType(unsigned int p)
{
    if(tokens->at(p) == "(") return tkLPARAN;
    if(tokens->at(p) == ")") return tkRPARAN;
    
    // Check for Hex Value
    if (tokens->at(p).compare(0, 2, "0x") == 0 && tokens->at(p).size() > 2 
        && tokens->at(p).find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos)
        return tkHEXNUMBER;

    if (tokens->at(p).find_first_not_of( "0123456789" ) == std::string::npos)
        return tkNUMBER;

    return tkSTRING;
}

bool MacroParser::isLast()
{
    if(pos == tokens->size())
        return true;
    return false;
}

bool MacroParser::isType(MacroParser::tkType type)
{
    if(pos >= tokens->size())
        return false;

    if(this->getType(pos) != type)
        return false;

    return true;
}

bool MacroParser::isNext(MacroParser::tkType type)
{
    std::cout << pos << " " << tokens->size() << std::endl;
    if(pos + 1 >= tokens->size())
        return false;
    
    if(getType(pos+1) != type)
        return false;

    return true;
}

Macro* MacroParser::parseMacro()
{
    std::string name, macro;

    do
    {
    // Accept Name and Hex Value
    if (isType(tkSTRING) && isNext(tkHEXNUMBER))
    {
        name = tokens->at(pos);  pos++;
        macro = tokens->at(pos); pos++;
        break;
    }

    if (isType(tkSTRING) && isNext(tkNUMBER))
    {
        name = tokens->at(pos);  pos++;
        macro = tokens->at(pos); pos++;
        break;
    }

    if (isType(tkSTRING) && isNext(tkSTRING))
    {
        name = tokens->at(pos);  pos++;
        macro = tokens->at(pos); pos++;

        macro = getMacroType(macro);

        break;
    }

    // Parse macro function
    if (isType(tkSTRING) && isNext(tkLPARAN))
    {
        name = tokens->at(pos);  pos++;
        macro = tokens->at(pos); pos++;

        macro = getMacroType(macro);

        break;
    }

    } while (0);
    
    return new Macro(name, macro);
}