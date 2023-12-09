#include "linkTable.h"

void linkTable::add(const teamId& newTeamId, const playerId& newPlayerId)
{
	const auto foundPlayer = playersInTeam[newTeamId].find(newPlayerId);
	if (foundPlayer == std::end(playersInTeam[newTeamId]))
	{
		playersInTeam[newTeamId].insert(newPlayerId);
		teamsPlayers[newPlayerId] = newTeamId;
	}
}

void linkTable::add(const matchId& newMatchId, const teamId& newTeam)
{
	auto& match = teamsInMatch[newMatchId];
	const auto foundPlayer = match.find(newTeam);
	if (foundPlayer == std::end(match) && match.size() <= impl::maxTeamInMatch)
	{
		match.insert(newTeam);
	}
}