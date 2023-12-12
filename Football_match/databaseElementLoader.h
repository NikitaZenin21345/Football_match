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
	Id insertUniqueElementInDataBase(Element& element)
	{
		auto elementId = footballMatchDataBase.findId(element);
		if (elementId == Id{})
		{
			elementId = Id(footballMatchDataBase.getNewId());
			element.setId(elementId);
			footballMatchDataBase.add(elementId, element);
		}
		return elementId;
	}

	void createPlayerAndLoadInDataBase(tokens& parameters) ;

	void createPlayerAndAddNewInDataBase(tokens& parameters) ;

	void addPlayer(tokens& parameters);

	void createTeamAndAddInDataBase(tokens& parameters);

	void createTeamAndLoadInDataBase(tokens& parameters) ;

	void addTeam(tokens& parameters);

	void createMatchAndLoadInDataBase(tokens& parameters) ;

	void createMatchAndAddInDataBase(tokens& parameters) ;

	void addMatch(tokens& parameters);

public:
	void setParserStrategy(const parserStrategy newStrategy) noexcept
	{
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
				throw parserException("Invalid element naming");
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

