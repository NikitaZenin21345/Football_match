#pragma once
#include <string>
#include "token.h"
class Parser 
{
public:
	virtual void parse() = 0;
	virtual ~Parser() = default;
};

class textFileParser final: public Parser
{
	std::istream& stream;
	std::string	forbidden = "\t";
	tokens parsedTokens {} ;
public:
	textFileParser(std::istream& stream_, const std::string& forbidden_ = "\t"): stream(stream_), forbidden(forbidden_){}
	void parse() override;
	tokens& getParsed();
};