#pragma once
#include <string>
#include <vector>
#include "token.h"

tokens parse_next_line(std::istream& stream,const std::string& forbidden = "\t");

class invalid_input_exception : std::exception
{
	char const* what() const override
	{
		return "Invalid parameters!";
	}
};
