#include "date.h"

bool date::isValidTime(const std::tm& timeStruct) noexcept
{
	if (timeStruct.tm_year < 0 || timeStruct.tm_mon < 0 || timeStruct.tm_mday < 1 ||
		timeStruct.tm_hour < 0 || timeStruct.tm_hour > 23 ||
		timeStruct.tm_min < 0 || timeStruct.tm_min > 59 ||
		timeStruct.tm_sec < 0 || timeStruct.tm_sec > 59) {
		return false;
	}
	return true;
}

std::string date::getDataInString() const
{
	constexpr int bufferSize = 30;
	char buffer[bufferSize];
	if (std::strftime(buffer, bufferSize, format, &time))
	{
		return std::string{ buffer };
	}
	return {};
}

bool date::operator==(const date& other) const
{
	return (time.tm_year == other.time.tm_year &&
		time.tm_mon == other.time.tm_mon &&
		time.tm_mday == other.time.tm_mday &&
		time.tm_hour == other.time.tm_hour &&
		time.tm_min == other.time.tm_min &&
		time.tm_sec == other.time.tm_sec);
}

std::tm date::getTime(const std::string& timeStr)
{
	const char* format = "%Y-%m-%d %H:%M:%S";
	std::tm time = {};
	std::istringstream dateStream(timeStr);
	dateStream >> std::get_time(&time, format);
	if (!isValidTime(time))
	{
		return std::tm{};
	}
	return time;
}

