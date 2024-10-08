#pragma once
#include <string>
#include "Id.h"


class teamId final : public ID
{
public:
	teamId() = default;
	explicit teamId(const boost::uuids::uuid& id_) : ID(id_) {}
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
	team() = default;
	team(const std::string& name_, const teamId& id_) : id(id_), name(name_){}
	[[nodiscard]] const std::string& getName() const noexcept { return name; }
	bool operator==(const team& other) const
	{
		return (name == other.name);
	}
	[[nodiscard]] const teamId& getId() const { return id; }
	void setId(const teamId& newId) noexcept { id = newId; }
};