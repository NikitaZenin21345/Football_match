#pragma once
#include <string>
#include "Id.h"

class matchId final : public ID
{
public:
	matchId() = default;
	explicit matchId(const size_t id_) : ID(id_) {}
	bool operator==(const matchId& otherId) const
	{
		return id == otherId.id;
	}
};

class match final
{
	using Result = int;
	using Data = int;
	Data date{};
	std::string place{};
	Result result{};
	
public:
	match(int date_, std::string& name_, int result_) : date(date_), place(std::move(name_)),  result(result_){}
	void setDate(const int date_) { date = date_; }
	void setResult(const int result_) { result = result_; }
	void setPlace(const std::string& place_) { place = place_; }
	[[nodiscard]] Data getDate() const { return date ; }
	[[nodiscard]] Result getResult() const {return result; }
	[[nodiscard]] const std::string& getPlace() const {return place; }
	bool operator==(const match& other) const
	{
		return date == other.date && place == other. place && result == other.result;
	}
};