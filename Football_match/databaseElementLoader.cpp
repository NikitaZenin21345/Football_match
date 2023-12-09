#include "databaseElementLoader.h"
#include "elementCreator.h"
namespace 
{
	const std::string matchName = "Match";
	const std::string teamName = "Team";
	const std::string playerName = "Player";
}

 databaseElementLoader::elementType  databaseElementLoader::getElementType(const std::string& type) const noexcept
{
	if (type == matchName)
	{
		return elementType::MATCH;
	}
	if (type == teamName)
	{
		return elementType::TEAM;
	}
	if (type == playerName)
	{
		return elementType::PLAYER;
	}
	return elementType::INVALID;
}

void databaseElementLoader::createPlayerAndLoadInDataBase(tokens& parameters) const
{
	const auto newPlayer = elementCreator::createPlayerFromToken(parameters, 2);
	const auto newPlayerId = elementCreator::createPlayerIdFromToken(parameters, 1);
	const auto playersTeamId = elementCreator::createTeamIdFromToken(parameters, 3);
	footballMatchDataBase.add(newPlayerId, newPlayer);
	footballMatchLinkTable.add(playersTeamId, newPlayerId);
}

void databaseElementLoader::createPlayerAndAddNewInDataBase(tokens& parameters) const
{
	const auto newPlayer = elementCreator::createPlayerFromToken(parameters, 1);
	const auto newPlayerId = insertInDataBase<playerId, player>(newPlayer);
	const auto playersTeam = elementCreator::createTeamFromToken(parameters, 2);
	const auto playersTeamId = insertInDataBase<teamId, team>(playersTeam);
	footballMatchLinkTable.add(playersTeamId, newPlayerId);
}

void  databaseElementLoader::addPlayer(tokens& parameters) const
{
	if (strategy == parserStrategy::loadElementInDataBase)
	{
		createPlayerAndLoadInDataBase(parameters);
	}
	else if (strategy == parserStrategy::addNewElementInDataBase)
	{
		createPlayerAndAddNewInDataBase(parameters);
	}
}

void databaseElementLoader::createTeamAndLoadInDataBase(tokens& parameters) const
{
	const auto newTeamId = elementCreator::createTeamIdFromToken(parameters, 1);
	const auto newTeam = elementCreator::createTeamFromToken(parameters, 2);
	footballMatchDataBase.add(newTeamId, newTeam);
}

void databaseElementLoader::addTeam(tokens& parameters) const
{
	if (strategy == parserStrategy::loadElementInDataBase)
	{
		createTeamAndLoadInDataBase(parameters);
	}
	else if (strategy == parserStrategy::addNewElementInDataBase)
	{
		createTeamAndAddInDataBase(parameters);
	}
}

void databaseElementLoader::createMatchAndLoadInDataBase(tokens& parameters) const
{
	const auto matchId_ = elementCreator::createMatchIdFromToken(parameters, 1);//вынести константы
	const auto newMatch = elementCreator::createMatchFromToken(parameters, 2, 3, 4);
	footballMatchDataBase.add(matchId_, newMatch);
	const auto firstTeamId = elementCreator::createTeamIdFromToken(parameters, 5);
	const auto secondTeamId = elementCreator::createTeamIdFromToken(parameters, 6);
	footballMatchLinkTable.add(matchId_, firstTeamId);
	footballMatchLinkTable.add(matchId_, secondTeamId);
}

void databaseElementLoader::createMatchAndAddInDataBase(tokens& parameters) const
{
	const auto newMatch = elementCreator::createMatchFromToken(parameters, 1, 2, 3);
	const auto firstTeam = elementCreator::createTeamFromToken(parameters, 4);
	const auto firstTeamId = insertInDataBase<teamId, team>(firstTeam);
	const auto secondTeam = elementCreator::createTeamFromToken(parameters, 5);
	const auto secondTeamId = insertInDataBase<teamId, team>(secondTeam);
	const auto newMatchId = footballMatchDataBase.add(newMatch);
	footballMatchLinkTable.add(newMatchId, firstTeamId);
	footballMatchLinkTable.add(newMatchId, secondTeamId);
}

void databaseElementLoader::addMatch(tokens& parameters) const
{
	if (strategy == parserStrategy::loadElementInDataBase)
	{
		createMatchAndLoadInDataBase(parameters);
	}
	else if (strategy == parserStrategy::addNewElementInDataBase)
	{
		createMatchAndAddInDataBase(parameters);
	}
}

bool databaseElementLoader::checkPlayersInTeam() const
{
	auto team = footballMatchDataBase.getTeamsListBeginning();
	while (team != footballMatchDataBase.getTeamsListEnd())
	{
		auto allPlayersInTeam = footballMatchLinkTable.getAllPlayersInTeam(team->first);
		if (!compareElementInLinkTable(allPlayersInTeam))
		{
			return false;
		}
		++team;
	}
	return true;
}

 bool databaseElementLoader::checkTeamInMatch() const
{
	auto match = footballMatchDataBase.getMatchesListBeginning();
	while (match != footballMatchDataBase.getMatchesListEnd())
	{
		auto teamsInMatches = footballMatchLinkTable.getTeamsInMatch(match->first);
		if (!compareElementInLinkTable(teamsInMatches))
		{
			return false;
		}
		++match;
	}
	return false;
}

void databaseElementLoader::addElement(std::istream& inputFile)
{
	tokens tokensList = parseNextLine(inputFile);
	while (tokensList.isValid())
	{
		addElement(tokensList);
		tokensList = parseNextLine(inputFile);
	}
}