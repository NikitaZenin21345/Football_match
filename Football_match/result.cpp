#include "result.h"

result result::getResultFromStr(const std::string& token)
{
	std::istringstream stringStream(token);
	auto tokens = parseNextLine(stringStream, ":");
	const auto firstScore = std::stoi(tokens.getTokens(0));
	const auto secondScore = std::stoi(tokens.getTokens(1));
	if (firstScore < 0 || secondScore < 0)
	{
		throw parserException("Wrong match result format");
	}
	return result{ static_cast<size_t>(firstScore), static_cast<size_t>(secondScore) };
}
