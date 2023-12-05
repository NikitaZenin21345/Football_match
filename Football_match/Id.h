#pragma once

class ID
{
protected:
	size_t id{};
public:
	ID() = default;
	ID(const size_t id_): id(id_){}
	void setId(const size_t id_) noexcept{ id = id_; }
	[[nodiscard]]size_t getId() const noexcept { return id; }
};

template<typename IdType>
	requires std::is_base_of_v<ID, IdType>
class idHash final
{
public:
	size_t operator()(const IdType& id) const noexcept
	{
		return std::hash<size_t>()(id.getId());
	}
};