#pragma once
#include <type_traits>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


class ID
{
protected:
	boost::uuids::uuid  id{};
public:
	ID() = default;
	ID(const boost::uuids::uuid& id_): id(id_){}
	void setId(const boost::uuids::uuid  id_) noexcept{ id = id_; }
	[[nodiscard]] boost::uuids::uuid  getId() const noexcept { return id; }
};

template<typename IdType>
	requires std::is_base_of_v<ID, IdType>
class idHash final
{
public:
	size_t operator()(const IdType& id) const noexcept
	{
		return boost::uuids::hash_value(id.getId());
	}
};
