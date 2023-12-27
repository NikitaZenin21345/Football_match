#include "databaseElementLoader.h"
#include "footballMatchObjectCreator.h"
namespace
{
	std::unordered_map<std::string, elementType> elements
	{
		{"Match", elementType::MATCH},
		{"Team", elementType::TEAM},
		{"Player", elementType::PLAYER},
	};
	constexpr size_t LoadInDataBaseElementIndex = 1;
	constexpr size_t LoadInDataBaseElementIdIndex = 2;
	constexpr size_t addInDataBaseElementIndex = 1;
}

elementType  databaseElementLoader::getElementType(const std::string& type) const noexcept
{
	const auto found_command = elements.find(type);
	if (found_command == elements.end())
	{
		return elementType::INVALID;
	}
	return found_command->second;
}

void databaseElementLoader::uploaExistingPlayer(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
{
	auto newPlayer = footballMatchObjectCreator::createPlayerFromToken(parameters, LoadInDataBaseElementIdIndex);
	const auto newPlayerId = footballMatchObjectCreator::createPlayerIdFromToken(parameters, LoadInDataBaseElementIndex);
	const auto playersTeamId = footballMatchObjectCreator::createTeamIdFromToken(parameters, LoadInDataBaseElementIdIndex + 1);
	footballMatchDataBase.add(newPlayerId, newPlayer);
	footballMatchLinkTable.add(playersTeamId, newPlayerId);
}

void databaseElementLoader::createNewPLayer(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
{
	auto newPlayer = footballMatchObjectCreator::createPlayerFromToken(parameters, addInDataBaseElementIndex);
	const auto newPlayerId = insertUniqueElementInDataBase<playerId, player>(newPlayer, footballMatchDataBase);
	auto playersTeam = footballMatchObjectCreator::createTeamFromToken(parameters, addInDataBaseElementIndex + 1);
	const auto playersTeamId = insertUniqueElementInDataBase<teamId, team>(playersTeam, footballMatchDataBase);
	footballMatchLinkTable.add(playersTeamId, newPlayerId);
}

void  databaseElementLoader::addPlayer(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
{
	if (strategy == parserStrategy::uploadExisting)
	{
		uploaExistingPlayer(parameters, footballMatchDataBase, footballMatchLinkTable);
	}
	else if (strategy == parserStrategy::addNewElement)
	{
		createNewPLayer(parameters, footballMatchDataBase, footballMatchLinkTable);
	}
}

void databaseElementLoader::createNewTeam(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
{
	auto newTeam = footballMatchObjectCreator::createTeamFromToken(parameters, addInDataBaseElementIndex);
	insertUniqueElementInDataBase<teamId, team>(newTeam, footballMatchDataBase);
}

void databaseElementLoader::uploadExistingTeam(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
{
	const auto newTeamId = footballMatchObjectCreator::createTeamIdFromToken(parameters, LoadInDataBaseElementIndex);
	auto newTeam = footballMatchObjectCreator::createTeamFromToken(parameters, LoadInDataBaseElementIdIndex);
	footballMatchDataBase.add(newTeamId, newTeam);
}

void databaseElementLoader::addTeam(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
{
	if (strategy == parserStrategy::uploadExisting)
	{
		uploadExistingTeam(parameters, footballMatchDataBase, footballMatchLinkTable);
	}
	else if (strategy == parserStrategy::addNewElement)
	{
		createNewTeam(parameters, footballMatchDataBase, footballMatchLinkTable);
	}
}

void databaseElementLoader::uploaExistingMatch(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
{
	const auto newMatchId = footballMatchObjectCreator::createMatchIdFromToken(parameters, LoadInDataBaseElementIndex);
	auto newMatch = footballMatchObjectCreator::createMatchFromToken(parameters, LoadInDataBaseElementIdIndex, LoadInDataBaseElementIdIndex+ 1, LoadInDataBaseElementIdIndex + 2);
	footballMatchDataBase.add(newMatchId, newMatch);
	const auto firstTeamId = footballMatchObjectCreator::createTeamIdFromToken(parameters, LoadInDataBaseElementIndex + 4);
	const auto secondTeamId = footballMatchObjectCreator::createTeamIdFromToken(parameters, LoadInDataBaseElementIndex + 5);
	footballMatchLinkTable.add(newMatchId, firstTeamId);
	footballMatchLinkTable.add(newMatchId, secondTeamId);
}

void databaseElementLoader::createNewMatch(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
{
	const auto newMatchId = matchId(footballMatchDataBase.getNewId());
	auto newMatch = footballMatchObjectCreator::createMatchFromToken(parameters, addInDataBaseElementIndex, addInDataBaseElementIndex + 1, addInDataBaseElementIndex + 2);
	footballMatchDataBase.add(newMatchId, newMatch);
	auto firstTeam = footballMatchObjectCreator::createTeamFromToken(parameters, 4);
	auto secondTeam = footballMatchObjectCreator::createTeamFromToken(parameters, 5);
	const auto firstTeamId = insertUniqueElementInDataBase<teamId, team>(firstTeam, footballMatchDataBase);
	const auto secondTeamId = insertUniqueElementInDataBase<teamId, team>(secondTeam, footballMatchDataBase);
	footballMatchLinkTable.add(newMatchId, firstTeamId);
	footballMatchLinkTable.add(newMatchId, secondTeamId);
}

void databaseElementLoader::addMatch(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
{
	if (strategy == parserStrategy::uploadExisting)
	{
		uploaExistingMatch(parameters, footballMatchDataBase, footballMatchLinkTable);
	}
	else if (strategy == parserStrategy::addNewElement)
	{
		createNewMatch(parameters, footballMatchDataBase, footballMatchLinkTable);
	}
}

void databaseElementLoader::addElement(std::istream& inputFile, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
{
	auto parser = textFileSeparatedBySimpleDelimitersParser(inputFile);
	tokens tokensList = parser.getParsed();
	while (tokensList.isValid())
	{
		addElement(tokensList, footballMatchDataBase, footballMatchLinkTable);
		tokensList = parser.getParsed();
	}
}