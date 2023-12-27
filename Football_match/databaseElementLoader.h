#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "dataBase.h"
#include "linkTable.h"
#include "token.h"

enum class parserStrategy { uploadExisting, addNewElement };
enum class elementType { PLAYER, TEAM, MATCH, INVALID };

class  databaseElementLoader final
{
	parserStrategy strategy;

	[[nodiscard]] elementType getElementType(const std::string& type) const noexcept;

	template<typename Id, typename Element>
	//req
	Id insertUniqueElementInDataBase(Element& element, footballMatchObjectDataBase& footballMatchDataBase)
	{
		auto elementId = footballMatchDataBase.findId(element);
		if (!elementId.has_value())
		{
			elementId = Id(footballMatchDataBase.getNewId());
			element.setId(elementId.value());
			footballMatchDataBase.add(elementId.value(), element);
		}
		return elementId.value();
	}

	void uploaExistingPlayer(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable);

	void createNewPLayer(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable);

	void addPlayer(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable);

	void createNewTeam(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable);

	void uploadExistingTeam(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable);

	void addTeam(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable);

	void uploaExistingMatch(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable);

	void createNewMatch(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable);

	void addMatch(tokens& parameters, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable);

public:
	void setParserStrategy(const parserStrategy newStrategy) noexcept
	{
		strategy = newStrategy;
	}

	databaseElementLoader(const parserStrategy strategy_) :
		strategy(strategy_) {}

	template<typename Token>
		requires std::is_convertible_v<Token, tokens>
	void addElement(Token&& tokensArray, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
	{
		try
		{
			const auto type = getElementType(tokensArray.getTokens(0));
			switch (type)
			{
			case elementType::MATCH:
			{
				addMatch(tokensArray, footballMatchDataBase, footballMatchLinkTable);
				break;
			}
			case elementType::TEAM:
			{
				addTeam(tokensArray, footballMatchDataBase, footballMatchLinkTable);
				break;
			}
			case elementType::PLAYER:
			{
				addPlayer(tokensArray, footballMatchDataBase, footballMatchLinkTable);
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

	void addElement(std::istream& inputFile, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable);
};

