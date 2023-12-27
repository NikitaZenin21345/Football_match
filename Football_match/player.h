#pragma once
#include <string>
#include "Id.h"

class playerId final: public ID
{
public:
	playerId() = default;
	explicit playerId(const boost::uuids::uuid& id_): ID(id_){}
	bool operator==(const playerId& other) const noexcept { return id == other.id; }
};

class player final
{
	std::string name;
	playerId id;

public:

	player() = default;
	player(const std::string& name_, const playerId& id_): name(name_), id(id_){}
	[[nodiscard]]const std::string& getName() const noexcept { return name; }
	bool operator==(const player& other) const
	{
		return name == other.name;
	}
	[[nodiscard]]const playerId& getId() const { return id; }
	void setId(const playerId& newId) noexcept { id = newId; }
};
