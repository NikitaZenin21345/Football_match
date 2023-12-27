#pragma once
#include <string>
#include <algorithm>
#include "team.h"
#include "token.h"
#include "match.h"
#include "player.h"

class footballMatchObjectCreator final
{
	static constexpr size_t minCountMatchParameters = 3;
	static constexpr size_t minCountMatchIdParameters = 1;
	static constexpr size_t minCountPlayerParameters = 1;
	static constexpr size_t minCountPlayerIdParameters = 1;
	static constexpr size_t minCountTeamParameters = 1;
	static constexpr size_t minCountTeamIdParameters = 1;
public:
	static playerId createPlayerIdFromToken(tokens& parameters,const size_t index)
	{
		if (parameters.isValid() && minCountPlayerIdParameters <= parameters.size())
		{
			return playerId{ boost::uuids::string_generator()((parameters.getTokens(index)))};
		}
		return playerId{};
	}
	static player createPlayerFromToken(tokens& parameters, const size_t index)
	{
		if (parameters.isValid() && minCountPlayerParameters <= parameters.size())
		{
			return player{ parameters.getTokens(index), playerId()};
		}
		return player{};
	}
	static teamId createTeamIdFromToken(tokens& parameters,const size_t index)
	{
		if (parameters.isValid() && minCountTeamIdParameters <= parameters.size())
		{
			return teamId{ boost::uuids::string_generator()((parameters.getTokens(index))) };
		}
		return teamId{};
	}
	static team createTeamFromToken(tokens& parameters,const size_t index)
	{
		if (parameters.isValid() && minCountTeamParameters <= parameters.size())
		{
			return team{ parameters.getTokens(index), teamId() };
		}
		return team{};
	}
	static matchId createMatchIdFromToken(tokens& parameters,const size_t index)
	{
		if (parameters.isValid() && minCountMatchIdParameters <= parameters.size())
		{
			return matchId{ boost::uuids::string_generator()((parameters.getTokens(index))) };
		}
		return matchId{};
	}
	static match createMatchFromToken(tokens& parameters, const size_t timeIndex, const size_t placeIndex, const size_t resultIndex)
	{
		if (parameters.isValid() && minCountMatchParameters <= parameters.size())
		{
			return match{ date::getTime(parameters.getTokens(timeIndex)), parameters.getTokens(placeIndex),
				result::getResultFromStr(parameters.getTokens(resultIndex)), matchId() };
		}
		return match{};
	}
};
