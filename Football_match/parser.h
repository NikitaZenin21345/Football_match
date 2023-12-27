#pragma once
#include <string>
#include "token.h"
class textFileParser
{
public:
	virtual void parse() = 0;
	virtual ~textFileParser() = default;
};

class textFileSeparatedBySimpleDelimitersParser final : public textFileParser
{
	std::istream& stream;
	std::string	forbidden = "\t";
	tokens parsedTokens {} ;
public:
	textFileSeparatedBySimpleDelimitersParser(std::istream& stream_, const std::string& forbidden_ = "\t"): stream(stream_), forbidden(forbidden_){}
	void parse() override;
	tokens& getParsed();
};