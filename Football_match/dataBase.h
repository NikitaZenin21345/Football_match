#pragma once
#include <unordered_map>
#include <optional>
#include <ranges>
#include "player.h"
#include "team.h"
#include "match.h"


class footballMatchObjectDataBase final
{
	boost::uuids::random_generator gen;
	std::unordered_map<playerId, player, idHash<playerId>> players;
	std::unordered_map<teamId, team, idHash<teamId>>  teams;
	std::unordered_map<matchId, match, idHash<matchId>> matches;

	template<typename Id, typename Element>
	[[nodiscard]] std::optional<Element> findElementInTable(const std::unordered_map<Id, Element, idHash<Id>>& elementsTable, const Id& id) const
	{
		auto it = elementsTable.find(id);
		if (it != elementsTable.end()) {
			return it->second;
		}
		return std::nullopt;
	}

	template<typename Id, typename Element>
	[[nodiscard]] std::optional<Id> findIdInTable
	(const std::unordered_map<Id, Element, idHash<Id>>& elementsTable, const Element& requiredElement) const
	{
		if (requiredElement != Element{})
		{
			for (const auto& element : elementsTable)
			{
				if (element.second == requiredElement)
				{
					return element.first;
				}
			}
		}
		return std::nullopt;
	}
public:
	footballMatchObjectDataBase() = default;
	footballMatchObjectDataBase(const footballMatchObjectDataBase&) = delete;
	footballMatchObjectDataBase(footballMatchObjectDataBase&&) = default;
	footballMatchObjectDataBase& operator=(const footballMatchObjectDataBase&) = delete;
	footballMatchObjectDataBase& operator=(footballMatchObjectDataBase&&) = default;
	~footballMatchObjectDataBase() = default;

	void add(const playerId& id, player& newPlayer) noexcept;

	void add(const teamId& id, team& newTeam) noexcept;

	void add(const matchId& id, match& newMatch) noexcept;

	[[nodiscard]] std::ranges::view auto getPlayers() const {
		return std::views::values(players);
	}

	[[nodiscard]] std::ranges::view auto getTeams() const {
		return std::views::values(teams);
	}

	[[nodiscard]] std::ranges::view auto getMatches() const {
		return std::views::values(matches);
	}

	[[nodiscard]] std::optional<player> findElementById(const playerId& id) const
	{
		return findElementInTable<playerId, player>(players, id);
	}

	[[nodiscard]] std::optional<team> findElementById(const teamId& id) const
	{
		return findElementInTable<teamId, team>(teams, id);
	}

	[[nodiscard]] std::optional<match> findElementById(const matchId& id) const
	{
		return findElementInTable<matchId, match>(matches, id);
	}

	[[nodiscard]] std::optional<playerId> findId(const player& element) const
	{
		return findIdInTable(players, element);
	}

	[[nodiscard]] std::optional<teamId> findId(const team& element) const
	{
		return findIdInTable(teams, element);
	}

	[[nodiscard]] std::optional<matchId> findId(const match& element) const
	{
		return findIdInTable(matches, element);
	}

	boost::uuids::uuid getNewId()
	{
		return gen();
	}
};