#pragma once
#include <string>
#include "Id.h"

class playerId final: public ID
{
public:
	playerId() = default;
	explicit playerId(const size_t id_): ID(id_){}
	bool operator==(const playerId& other) const noexcept { return id == other.id; }
};

class player final
{
	std::string name;
public:
	player() = default;
	player( std::string& name_): name(std::move(name_)){}
	[[nodiscard]]const std::string& getName() const noexcept { return name; }
	bool operator==(const player& other) const
	{
		return name == other.name;
	}
};
