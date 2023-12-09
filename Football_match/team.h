#pragma once
#include <string>
#include "Id.h"


class teamId final : public ID
{
public:
	teamId() = default;
	explicit teamId(const size_t id_) : ID(id_) {}
	bool operator==(const teamId& otherId) const
	{
		return id == otherId.id;
	}
};

class team final
{
	teamId id;
	std::string name;
public:
	team() = default;//delete
	team(std::string& name_, const teamId& id_) : id(id_), name(std::move(name_)){}
	[[nodiscard]] const std::string& getName() const noexcept { return name; }
	bool operator==(const team& other) const
	{
		return name == other.name;
	}
	[[nodiscard]] const teamId& getId() const { return id; }
};