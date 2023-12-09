#pragma once
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "parserError.h"

class date final
{
	const char* format = "%Y-%m-%d %H:%M:%S";
	std::tm time{};
	static [[nodiscard]] bool isValidTime(const std::tm& timeStruct) noexcept;

public:
	date(const std::tm& time_): time(time_){}
	std::string getDataInString() const;
	bool operator==(const date& other) const;
	static std::tm getTime(const std::string& timeStr);
};
