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

void databaseElementLoader::createPlayerAndLoadInDataBase(tokens& parameters) 
{
	auto newPlayer = elementCreator::createPlayerFromToken(parameters, 2);
	const auto newPlayerId = elementCreator::createPlayerIdFromToken(parameters, 1);
	newPlayer.setId(newPlayerId);
	const auto playersTeamId = elementCreator::createTeamIdFromToken(parameters, 3);
	footballMatchDataBase.add(newPlayerId, newPlayer);
	footballMatchLinkTable.add(playersTeamId, newPlayerId);
}

void databaseElementLoader::createPlayerAndAddNewInDataBase(tokens& parameters) 
{
	auto newPlayer = elementCreator::createPlayerFromToken(parameters, 1);
	const auto newPlayerId = insertUniqueElementInDataBase<playerId, player>(newPlayer);
	auto playersTeam = elementCreator::createTeamFromToken(parameters, 2);
	const auto playersTeamId = insertUniqueElementInDataBase<teamId, team>(playersTeam);
	footballMatchLinkTable.add(playersTeamId, newPlayerId);
}

void  databaseElementLoader::addPlayer(tokens& parameters) 
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

void databaseElementLoader::createTeamAndAddInDataBase(tokens& parameters)
{
	auto newTeam = elementCreator::createTeamFromToken(parameters, 1);
	insertUniqueElementInDataBase<teamId, team>(newTeam);
}

void databaseElementLoader::createTeamAndLoadInDataBase(tokens& parameters) 
{
	const auto newTeamId = elementCreator::createTeamIdFromToken(parameters, 1);
	auto newTeam = elementCreator::createTeamFromToken(parameters, 2);
	newTeam.setId(newTeamId);
	footballMatchDataBase.add(newTeamId, newTeam);
}

void databaseElementLoader::addTeam(tokens& parameters) 
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

void databaseElementLoader::createMatchAndLoadInDataBase(tokens& parameters) 
{
	const auto newMatchId = elementCreator::createMatchIdFromToken(parameters, 1);//вынести константы
	auto newMatch = elementCreator::createMatchFromToken(parameters, 2, 3, 4);
	newMatch.setId(newMatchId);
	footballMatchDataBase.add(newMatchId, newMatch);
	const auto firstTeamId = elementCreator::createTeamIdFromToken(parameters, 5);
	const auto secondTeamId = elementCreator::createTeamIdFromToken(parameters, 6);
	footballMatchLinkTable.add(newMatchId, firstTeamId);
	footballMatchLinkTable.add(newMatchId, secondTeamId);
}

void databaseElementLoader::createMatchAndAddInDataBase(tokens& parameters) 
{
	const auto newMatchId = matchId(footballMatchDataBase.getNewId());
	auto newMatch = elementCreator::createMatchFromToken(parameters, 1, 2, 3);
	newMatch.setId(newMatchId);
	footballMatchDataBase.add(newMatchId, newMatch);
	auto firstTeam = elementCreator::createTeamFromToken(parameters, 4);
	auto secondTeam = elementCreator::createTeamFromToken(parameters, 5);
	const auto firstTeamId = insertUniqueElementInDataBase<teamId, team>(firstTeam);
	const auto secondTeamId = insertUniqueElementInDataBase<teamId, team>(secondTeam);
	footballMatchLinkTable.add(newMatchId, firstTeamId);
	footballMatchLinkTable.add(newMatchId, secondTeamId);
}

void databaseElementLoader::addMatch(tokens& parameters) 
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

void databaseElementLoader::addElement(std::istream& inputFile)
{
	auto parser = textFileParser(inputFile);
	tokens tokensList = parser.getParsed();
	while (tokensList.isValid())
	{
		addElement(tokensList);
		tokensList = parser.getParsed();
	}
}