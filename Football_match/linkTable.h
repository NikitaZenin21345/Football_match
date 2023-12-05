#pragma once
#include <unordered_map>
#include <memory>
#include "Team.h"
#include "player.h"
#include "match.h"

namespace impl
{
	constexpr size_t maxTeamInMatch = 2;
}

class linkTable final
{
	std::unordered_map<teamId, std::unordered_set<playerId, idHash<playerId>>,idHash<teamId>> playersInTeam{};
	std::unordered_map<playerId, teamId, idHash<playerId>> teamsPlayers{};
	std::unordered_map<matchId, std::unordered_set<teamId, idHash<teamId>>, idHash<matchId>> teamsInMatch{};

public:

	void add(const teamId& newTeamId, const playerId& newPlayerId)
	{
		const auto foundPlayer =  playersInTeam[newTeamId].find(newPlayerId);
		if(foundPlayer == std::end(playersInTeam[newTeamId]))
		{
			playersInTeam[newTeamId].insert(newPlayerId);
			teamsPlayers[newPlayerId] = newTeamId;
		}
	}

	void add(const matchId& newMatchId, const teamId& newTeam)
	{
		auto& match = teamsInMatch[newMatchId];
		const auto foundPlayer = match.find(newTeam);
		if (foundPlayer == std::end(match) && match.size() <= impl::maxTeamInMatch)
		{
			match.insert(newTeam);
		}
	}

	[[nodiscard]]decltype(auto) getAllPlayersInTeam(const teamId& id) const
	{
		const auto foundElement = playersInTeam.find(id);
		return foundElement == playersInTeam.end() ? throw/* */: foundElement->second;
	}

	[[nodiscard]] decltype(auto) getTeamsInMatch(const matchId& id) const
	{
		const auto foundElement = teamsInMatch.find(id);
		return foundElement == teamsInMatch.end() ? throw/* */: foundElement->second;
	}

	[[nodiscard]] decltype(auto) getPlayerTeam(const playerId& id) const
	{
		const auto foundElement = teamsPlayers.find(id);
		return foundElement == teamsPlayers.end() ? throw/* */ : foundElement->second;
	}
};

