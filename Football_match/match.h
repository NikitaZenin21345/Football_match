#pragma once
#include <string>
#include <ctime>
#include "Id.h"
#include "result.h"
#include "date.h"

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
	date matchDate;
	std::string place;
	result matchResult;
	matchId id;
public:
	match(const std::tm& date_, std::string& place_,const result& result_, const matchId& id_) : matchDate(date_),
		place(std::move(place_)), matchResult(result_), id(id_){}
	void setDate(const date& date_)
	{
		matchDate = date_;
	}
	void setResult(const result result_) { matchResult = result_; }
	void setPlace(const std::string& place_) { place = place_; }
	[[nodiscard]] std::string getDate() const
	{
		return matchDate.getDataInString();
	}
	[[nodiscard]] result getResult() const {return matchResult; }
	[[nodiscard]] const std::string& getPlace() const {return place; }
	bool operator==(const match& other) const
	{
		return matchDate == other.matchDate && place == other.place && matchResult == other.matchResult;
	}
	[[nodiscard]] const matchId& getId() const { return id; }
};