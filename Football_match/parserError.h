#pragma once
#include <exception>
#include <string>

class parserException: public std::exception
{
	std::string massege;
public:
	parserException(const std::string& massage_): massege(massage_){}
	char const* what() const override
	{
		return massege.c_str();
	}
};
