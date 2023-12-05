#pragma once
#include <string>
#include "dataBase.h"
#include "linkTable.h"
#include "parser.h"
#include "token.h"

namespace impl
{
	const std::string matchName = "Match";
	const std::string teamName = "Team";
	const std::string playerName = "Player";
}

enum class parserStrategy {loadNewElementInDataBase, addNewElementInDataBase};

class elementCreator final
{
	enum class elementType { PLAYER, TEAM, MATCH, NOVALID };
	dataBase& footballMatchDataBase;
	linkTable& footballMatchLinkTable;
	parserStrategy strategy;

	[[nodiscard]] elementType getElementType(const std::string& type) const noexcept
	{
		if (type == impl::matchName)
		{
			return elementType::MATCH;
		}
		if (type == impl::teamName)
		{
			return elementType::TEAM;
		}
		if (type == impl::playerName)
		{
			return elementType::PLAYER;
		}
		return elementType::NOVALID;
	}

	template<typename Id, typename Element>
	Id getElementId(const Element& element)
	{
		auto elementId = footballMatchDataBase.findId(element);
		if (elementId == Id{})
		{
			elementId = footballMatchDataBase.add(element);
		}
		return elementId;
	}

	void addPlayer(tokens& parameters)//добавить стратегию для парсера через шаблон с енумом, выдавать id в базе данных, здесь делать поиск 
	{
		if(strategy == parserStrategy::loadNewElementInDataBase)
		{
			const auto newPlayer = player(parameters.getTokens(2));
			const auto newPlayerId = playerId(std::stoi(parameters.getTokens(1)));
			const auto playersTeamId = teamId(std::stoi(parameters.getTokens(3)));
			footballMatchDataBase.add(newPlayerId, newPlayer);
			footballMatchLinkTable.add(playersTeamId, newPlayerId);
		}
		else
		{
			const auto newPlayer = player(parameters.getTokens(1));
			const auto newPlayerId = getElementId<playerId, player>(newPlayer);
			const auto playersTeam = team(parameters.getTokens(2));
			const auto playersTeamId = getElementId<teamId, team>(playersTeam);
			footballMatchDataBase.add(newPlayer);
			footballMatchLinkTable.add(playersTeamId, newPlayerId);
		}

	}

	void addTeam(tokens& parameters) 
	{
		const auto teamId_ = teamId(std::stoi(parameters.getTokens(1)));
		const auto team_ = team(parameters.getTokens(2));
		footballMatchDataBase.add(teamId_, team_);
	}


	void addMatch(tokens& parameters) 
	{
		const auto matchId_ = matchId(std::stoi(parameters.getTokens(1)));
		const auto match_ = match(std::stoi(parameters.getTokens(2)), parameters.getTokens(3), std::stoi(parameters.getTokens(4)));
		footballMatchDataBase.add(matchId_, match_);
		const auto firstTeamId = teamId(std::stoi(parameters.getTokens(5)));
		const auto secondTeamId = teamId(std::stoi(parameters.getTokens(6)));
		footballMatchLinkTable.add(matchId_, firstTeamId);
		footballMatchLinkTable.add(matchId_, secondTeamId);
	}

	[[nodiscard]]bool checkTable() const
	{
		auto team = footballMatchDataBase.getTeamsListBeginning();
		while (team != footballMatchDataBase.getTeamsListEnd())
		{
			try
			{
				auto allPlayersInTeam = footballMatchLinkTable.getAllPlayersInTeam(team->first);
				auto playerInTeam = allPlayersInTeam.begin();
				while (playerInTeam != allPlayersInTeam.end())
				{
					if (footballMatchDataBase.findElement(*playerInTeam) == nullptr)
					{
						return false;
					}
					++playerInTeam;
				}
				++team;
			}
			catch (...)
			{
				//
				break;
			}
		}
		return false;
	}

	[[nodiscard]]bool checkIntegrity() const noexcept
	{
		return checkTable();
	}

public:
	void setStrategy(const parserStrategy newStrategy)
	{
		strategy = newStrategy;
	}

	elementCreator(dataBase& footballMatchDataBase_, linkTable& footballMatchLinkTable_, const parserStrategy strategy_) :
		footballMatchDataBase(footballMatchDataBase_), footballMatchLinkTable(footballMatchLinkTable_), strategy(strategy_) {}

	template<typename Token>
		requires std::is_convertible_v<Token, tokens>
	void addElement(Token&& tokensArray)
	{
		const auto type = getElementType(tokensArray.getTokens(0));
		switch (type)
		{
			case elementType::MATCH:
			{
				addMatch(tokensArray);
				break;
			}
			case elementType::TEAM:
			{
				addTeam(tokensArray);
				break;
			}
			case elementType::PLAYER:
			{
				addPlayer(tokensArray);
				break;
			}
			case elementType::NOVALID:
			{
				throw;
			}
		}
	}

	void addElement(std::ifstream& inputFile)
	{
		tokens tokensList = parse_next_line(inputFile);
		while (tokensList.isValid())
		{
			addElement(tokensList);
			tokensList = parse_next_line(inputFile);
		}
	}

	dataBase& createDataBase() noexcept
	{
		return footballMatchDataBase;//delete
	}

	linkTable& createLinkTable() noexcept
	{
		return footballMatchLinkTable;//toge
	}
};