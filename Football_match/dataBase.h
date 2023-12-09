#pragma once
#include <unordered_map>
#include <memory>
#include "player.h"
#include "team.h"
#include "match.h"

class dataBase final
{
	std::unordered_map<playerId, std::shared_ptr<player>, idHash<playerId>> players;
	std::unordered_map<teamId, std::shared_ptr<team>, idHash<teamId>>  teams;
	std::unordered_map<matchId, std::shared_ptr<match>, idHash<matchId>> matches;

	template<typename Id, typename Element>
	size_t getFreeId(const std::unordered_map<Id, std::shared_ptr<Element>, idHash<Id>>& elements)
	{
		size_t maxId = 0;
		for (const auto& element : elements)
		{
			auto elementId = element.first.getId();
			if (elementId > maxId)
			{
				maxId = elementId;
			}
		}
		return maxId + 1;
	}

	template<typename Id, typename Element>
	[[nodiscard]] std::shared_ptr<Element> findElementInTable
	(const std::unordered_map<Id, std::shared_ptr<Element>, idHash<Id>>& elementsTable, const Id& id) const
	{
		const auto foundElement = elementsTable.find(id);
		return foundElement == elementsTable.end() ? nullptr : foundElement->second;
	}

	template<typename Id, typename Element>
	[[nodiscard]] Id findIdInTable
	(const std::unordered_map<Id, std::shared_ptr<Element>, idHash<Id>>& elementsTable, const Element& requiredElement) const
	{
		for (const auto& element : elementsTable)
		{
			if (*element.second == requiredElement)
			{
				return element.first;
			}
		}
		return Id{};
	}//guid id ������
public:

	[[nodiscard]] decltype(auto) getPlayersListBeginning()const { return players.begin(); }//������
	[[nodiscard]] decltype(auto) getTeamsListBeginning()const { return teams.begin(); }
	[[nodiscard]] decltype(auto) getMatchesListBeginning()const { return matches.begin(); }
	[[nodiscard]] decltype(auto) getPlayersListEnd()const { return players.end(); }
	[[nodiscard]] decltype(auto) getTeamsListEnd()const { return teams.end(); }
	[[nodiscard]] decltype(auto) getMatchesListEnd()const { return matches.end(); }

	void add(const playerId& id, const player& newPlayer) noexcept;

	playerId add(const player& newPlayer) noexcept;

	void add(const teamId& Id, const team& newTeam) noexcept;

	teamId add(const team& newTeam) noexcept;

	void add(const matchId& Id, const match& newMatch) noexcept;

	matchId add(const match& newMatch) noexcept;

	[[nodiscard]] std::shared_ptr<player> findElement(const playerId& id) const;

	[[nodiscard]] playerId findId(const player& element) const noexcept;

	[[nodiscard]] std::shared_ptr<team> findElement(const teamId& id) const;

	[[nodiscard]] teamId findId(const team& element) const noexcept;

	[[nodiscard]] std::shared_ptr<match> findElement(const matchId& id) const noexcept;

	[[nodiscard]] matchId findId(const match& element) const noexcept;

};