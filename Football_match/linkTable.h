#pragma once
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include "Team.h"
#include "player.h"
#include "match.h"
#include "dataBaseError.h"

namespace impl
{
	constexpr size_t maxTeamInMatch = 2;
}

class linkIdTable final
{
	std::unordered_map<teamId, std::unordered_set<playerId, idHash<playerId>>, idHash<teamId>> playersInTeam{};
	std::unordered_map<playerId, teamId, idHash<playerId>> teamsPlayers{};
	std::unordered_map<matchId, std::unordered_set<teamId, idHash<teamId>>, idHash<matchId>> teamsInMatch{};

public:

	void add(const teamId& newTeamId, const playerId& newPlayerId);

	void add(const matchId& newMatchId, const teamId& newTeam);

	[[nodiscard]] auto getAllPlayersInTeam(const teamId& id) const
	{
		const auto foundElement = playersInTeam.find(id);
		return foundElement == playersInTeam.end() ? std::nullopt : std::optional{ foundElement->second };
	}

	[[nodiscard]] auto getTeamsInMatch(const matchId& id) const
	{
		const auto foundElement = teamsInMatch.find(id);
		return foundElement == teamsInMatch.end() ? std::nullopt : std::optional{ foundElement->second };
	}

	[[nodiscard]] auto getPlayerTeam(const playerId& id) const
	{
		const auto foundElement = teamsPlayers.find(id);
		return foundElement == teamsPlayers.end() ?  std::nullopt : std::optional{ foundElement->second };
	}
};

