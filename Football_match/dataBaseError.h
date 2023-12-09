#pragma once
#include <exception>
#include <string>

class noFoundTableElement : public std::exception
{
	std::string massage;
public:
	noFoundTableElement(const std::string& massage_) : massage(massage_) {}
	char const* what() const override
	{
		return massage.c_str();
	}
};
