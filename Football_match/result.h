#pragma once
#include <sstream>
#include "parser.h"
class result final
{
	size_t firstCommandScore = std::numeric_limits<size_t>::max();
	size_t secondCommandScore =  std::numeric_limits<size_t>::max();
public:
	result() = default;
	result(const size_t firstScore, const size_t secondScore):firstCommandScore(firstScore), secondCommandScore(secondScore){}
	[[nodiscard]] size_t getFirstScore() const noexcept{ return firstCommandScore; }
	[[nodiscard]] size_t getSecondScore() const  noexcept{ return secondCommandScore; }
	void setFirstScore(const size_t score) noexcept{ firstCommandScore = score; }
	void setSecondScore(const size_t score) noexcept{ secondCommandScore = score; }
	bool operator==(const result&) const = default;

	static [[nodiscard]] result getResultFromStr(const std::string& token);
};
