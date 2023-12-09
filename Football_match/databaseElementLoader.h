#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "dataBase.h"
#include "linkTable.h"
#include "parser.h"
#include "token.h"



enum class parserStrategy { loadElementInDataBase, addNewElementInDataBase };

class  databaseElementLoader final
{
	enum class elementType { PLAYER, TEAM, MATCH, INVALID };
	dataBase& footballMatchDataBase;
	linkTable& footballMatchLinkTable;
	parserStrategy strategy;

	[[nodiscard]] elementType getElementType(const std::string& type) const noexcept;

	template<typename Id, typename Element>
	//req
	Id insertInDataBase(const Element& element) const
	{
		auto elementId = footballMatchDataBase.findId(element);
		if (elementId == Id{})
		{
			elementId = footballMatchDataBase.add(element);
		}
		return elementId;
	}

	void createTeamAndAddInDataBase(tokens& parameters) const
	{
		insertInDataBase<teamId, team>(team(parameters.getTokens(1), teamId(0)));
	}

	void createPlayerAndLoadInDataBase(tokens& parameters) const;

	void createPlayerAndAddNewInDataBase(tokens& parameters) const;

	void addPlayer(tokens& parameters) const;

	void createTeamAndLoadInDataBase(tokens& parameters) const;

	void addTeam(tokens& parameters) const;

	void createMatchAndLoadInDataBase(tokens& parameters) const;

	void createMatchAndAddInDataBase(tokens& parameters) const;

	void addMatch(tokens& parameters) const;

	template<typename Id>
	//requires
	[[nodiscard]] bool compareElementInLinkTable(const std::unordered_set<Id, idHash<Id>>& allElement) const
	{
		auto element = allElement.begin();
		while (element != allElement.end())
		{
			if (footballMatchDataBase.findElement(*element) == nullptr)
			{
				return false;
			}
			++element;
		}
		return true;
	}
	[[nodiscard]] bool checkPlayersInTeam() const;

	[[nodiscard]] bool checkTeamInMatch() const;

	[[nodiscard]] bool checkIntegrity() const noexcept
	{
		return checkPlayersInTeam() && checkTeamInMatch();
	}

public:
	void setParserStrategy(const parserStrategy newStrategy)
	{
		//checkIntegrity();
		strategy = newStrategy;
	}

	 databaseElementLoader(dataBase& footballMatchDataBase_, linkTable& footballMatchLinkTable_, const parserStrategy strategy_) :
		footballMatchDataBase(footballMatchDataBase_), footballMatchLinkTable(footballMatchLinkTable_), strategy(strategy_) {}

	template<typename Token>
		requires std::is_convertible_v<Token, tokens>
	void addElement(Token&& tokensArray)
	{
		try
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
			case elementType::INVALID:
			{
				throw parserException("No valid element naming");
			}
			}
		}
		catch (const std::exception& error)
		{
			std::cout << error.what() << std::endl;
		}
	}

	void addElement(std::istream& inputFile);
};

