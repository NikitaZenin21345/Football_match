#pragma once
#include <string>
#include "team.h"
#include "token.h"
#include "match.h"
#include "player.h"
class elementCreator final
{
public:
	static playerId createPlayerIdFromToken(tokens& parameters, size_t index)
	{
		return playerId{ static_cast<size_t>(std::stoi(parameters.getTokens(index))) };
	}
	static player createPlayerFromToken(tokens& parameters, size_t index)
	{
		return player{ parameters.getTokens(index), playerId(0)};
	}
	static teamId createTeamIdFromToken(tokens& parameters, size_t index)
	{
		return teamId{static_cast<size_t>(std::stoi(parameters.getTokens(index)))};
	}
	static team createTeamFromToken(tokens& parameters, size_t index)
	{
		return team{ parameters.getTokens(index), teamId(0)};
	}
	static matchId createMatchIdFromToken(tokens& parameters, size_t index)
	{
		return matchId{ static_cast<size_t>(std::stoi(parameters.getTokens(index))) };
	}
	static match createMatchFromToken(tokens& parameters,const size_t timeIndex,const size_t placeIndex,const size_t resultIndex)
	{
		return match{date::getTime(parameters.getTokens(timeIndex)), parameters.getTokens(placeIndex), result::getResultFromStr(parameters.getTokens(resultIndex)), matchId(0)};
	}
};
