#include <string>
#include <vector>
#include <fstream>
#include "parser.h"

//constexpr  std::string text_forbidden = ("\t");

static size_t add_token(const std::string& input_str, const size_t start_position, std::vector<std::string>& tokens, const std::string& forbidden)
{
	size_t start = input_str.find_first_not_of(forbidden, start_position);
	if (start == std::string::npos)
	{
		return std::string::npos;
	}
	size_t end = input_str.find_first_of(forbidden, start + 1);
	if (end == std::string::npos)
	{
		end = input_str.size();
	}
	std::string substr = input_str.substr(start, end - start);
	tokens.emplace_back(std::move(substr));
	return end;
}

tokens parseNextLine(std::istream& stream,const std::string& forbidden )
{
	size_t index = 0;
	std::string input_str;
	std::vector<std::string> tokensList;
	if (std::getline(stream, input_str))
	{
		while (index != std::string::npos)
		{
			index = add_token(input_str, index, tokensList, forbidden);
		}
		return tokens{ std::move(tokensList), true };
	}
	return tokens{ std::move(tokensList), false };
}
