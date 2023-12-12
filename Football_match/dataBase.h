#pragma once
#include <unordered_map>
#include <memory>
#include "player.h"
#include "team.h"
#include "match.h"

class dataBase final
{
	boost::uuids::random_generator gen;
	std::unordered_map<playerId, player, idHash<playerId>> players;
	std::unordered_map<teamId, team, idHash<teamId>>  teams;
	std::unordered_map<matchId, match, idHash<matchId>> matches;

	template<typename Id, typename Element>
	[[nodiscard]] Element findElementInTable(const std::unordered_map<Id, Element, idHash<Id>>& elementsTable, const Id& id) const
	{
		const auto foundElement = elementsTable.find(id);
		if (foundElement != elementsTable.end())
		{
			return foundElement->second;
		}
		return Element{};
	}

	template<typename Id, typename Element>
	[[nodiscard]] Id findIdInTable
	(const std::unordered_map<Id, Element, idHash<Id>>& elementsTable, const Element& requiredElement) const
	{
		if(requiredElement != Element{})
		{
			for (const auto& element : elementsTable)
			{
				if (element.second == requiredElement)
				{
					return element.first;
				}
			}
		}
		return Id{};
	}//guid id уюрать
public:

	[[nodiscard]] auto& getPlayers()const { return players; }
	[[nodiscard]] auto& getTeams()const { return teams; }
	[[nodiscard]] auto& getMatches()const { return matches; }

	void add(const playerId& id, const player& newPlayer) noexcept;

	void add(const teamId& Id, const team& newTeam) noexcept;

	void add(const matchId& Id, const match& newMatch) noexcept;

	[[nodiscard]] player findElement(const playerId& id) const
	{
		return findElementInTable<playerId, player>(players, id);
	}

	[[nodiscard]] team findElement(const teamId& id) const
	{
		return findElementInTable<teamId, team>(teams, id);
	}

	[[nodiscard]] match findElement(const matchId& id) const
	{
		return findElementInTable<matchId, match>(matches, id);
	}

	[[nodiscard]] playerId findId(const player& element) const noexcept
	{
		return findIdInTable(players, element);
	}

	[[nodiscard]] teamId findId(const team& element) const noexcept
	{
		return findIdInTable(teams, element);
	}

	[[nodiscard]] matchId findId(const match& element) const noexcept
	{
		return findIdInTable(matches, element);
	}
	
	boost::uuids::uuid getNewId() 
	{
		return gen();
	}
};