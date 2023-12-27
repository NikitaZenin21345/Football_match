#include <fstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ñhampionshipDataStorageSystem.h"
#include "databaseElementLoader.h"
#include "footballMatchObjectCreator.h"
#include "dataBaseSaver.h"

namespace
{
	enum class operations { addData, readData, Invalid };
	enum class addDataOperation {  addFromConsole , addFromFile , Invalid};
	enum class readDataOperation { getTeamPlayers , getMatchTeams , getPlayerTeam, Invalid};

	template< typename Element>
	void print(const Element& element);

	template<>
	void print(const match& element)
	{
		std::cout << element.getDate() << "\t";
		std::cout << element.getResult().getFirstScore() << ":" << element.getResult().getSecondScore() << "\t";
		std::cout << element.getPlace() << std::endl;
	}

	template<>
	void print(const player& element)
	{
		std::cout << element.getName() << std::endl;
	}

	template<>
	void print(const team& element)
	{
		std::cout << element.getName() << std::endl;
	}

	template<typename Id, typename Element>
	void printElements(const std::unordered_set<Id, idHash<Id>>& elements, const footballMatchObjectDataBase& footballMatchDataBase)
	{
		for (const auto& element : elements)
		{
			print<Element>(footballMatchDataBase.findElementById(element).value());
		}
	}

	void saveData(std::ifstream& dataFile, const std::string& dataSaveFileName,const footballMatchObjectDataBase& footballMatchDataBase,
		const linkIdTable& footballMatchLinkTable)
	{
		dataFile.close();
		std::ofstream saveFile(dataSaveFileName);
		if (!saveFile.is_open()) {
			std::cout << "Cannot open file" << std::endl;
			return;
		}
		const dataBaseSaver saver;
		saver.save(saveFile, footballMatchDataBase, footballMatchLinkTable);
		saveFile.close();
	}

	void addElementFromString(databaseElementLoader& Creator, std::string& inputLine,  footballMatchObjectDataBase& footballMatchDataBase,  linkIdTable& footballMatchLinkTable)
	{
		std::getline(std::cin, inputLine);
		std::istringstream stringStream(inputLine);
		auto parser = textFileSeparatedBySimpleDelimitersParser(stringStream);
		Creator.addElement(parser.getParsed(), footballMatchDataBase, footballMatchLinkTable);
	}

	void addElementFromFile(databaseElementLoader& Creator, std::string& inputLine, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
	{
		std::getline(std::cin, inputLine);
		std::ifstream sourceFile(inputLine);
		if (!sourceFile.is_open()){
			std::cerr << "Cannot open file" << std::endl;
			return;
		}
		Creator.addElement(sourceFile, footballMatchDataBase, footballMatchLinkTable);
	}

	void readTeamPlayers(std::string& inputLine,
		const footballMatchObjectDataBase& footballMatchDataBase, const linkIdTable& footballMatchLinkTable)
	{
		std::cout << "Input team parameters:" << std::endl;
		std::getline(std::cin, inputLine);
		std::istringstream stringStream(inputLine);
		auto parser = textFileSeparatedBySimpleDelimitersParser(stringStream);
		const auto searchedTeam = footballMatchObjectCreator::createTeamFromToken(parser.getParsed(), 0);
		const auto searchedTeamId = footballMatchDataBase.findId(searchedTeam);
		if (searchedTeamId.has_value())
		{
			printElements<playerId, player>(footballMatchLinkTable.getAllPlayersInTeam(searchedTeamId.value()).value(), footballMatchDataBase);
		}
	}

	void readMatchTeams(std::string& inputLine,
		const footballMatchObjectDataBase& footballMatchDataBase, const linkIdTable& footballMatchLinkTable)
	{
		std::cout << "Input Match parameters:" << std::endl;
		std::getline(std::cin, inputLine);
		std::istringstream stringStream(inputLine);
		auto parser = textFileSeparatedBySimpleDelimitersParser(stringStream);
		const auto searchedTeam = footballMatchObjectCreator::createMatchFromToken(parser.getParsed(), 0, 1, 2);
		const auto searchedMatchId = footballMatchDataBase.findId(searchedTeam);
		if (searchedMatchId.has_value())
		{
			printElements<teamId, team>(footballMatchLinkTable.getTeamsInMatch(searchedMatchId.value()).value(), footballMatchDataBase);
		}
	}

	void readPlayerTeam(std::string& inputLine,
		const footballMatchObjectDataBase& footballMatchDataBase, const linkIdTable& footballMatchLinkTable)
	{
		std::cout << "Input player parameters:" << std::endl;
		std::getline(std::cin, inputLine);
		std::istringstream stringStream(inputLine);
		auto parser = textFileSeparatedBySimpleDelimitersParser(stringStream);
		const auto searchedPlayer = footballMatchObjectCreator::createPlayerFromToken(parser.getParsed(), 0);
		const auto searchedPLayerId = footballMatchDataBase.findId(searchedPlayer);
		if (searchedPLayerId.has_value())
		{
			std::cout << footballMatchDataBase.findElementById(footballMatchLinkTable.getPlayerTeam(searchedPLayerId.value()).value())->getName() << std::endl;
		}
	}

	void printReadElementSwitchDialogWindow() noexcept
	{
		std::cout << "Switch option\n";
		std::cout << "0.Get a list of all the team's players\n";
		std::cout << "1.Get a match team's \n";
		std::cout << "2.Get a team player`s" << std::endl;
	}

	addDataOperation chooseOperationToAddNewElementDatabase(const int answer)
	{
		if (answer == 0)
		{
			return addDataOperation::addFromConsole;
		}
		if (answer == 1)
		{
			return addDataOperation::addFromFile;
		}
		return addDataOperation::Invalid;
	}

	readDataOperation chooseOperationToReadDatabase(const int answer)
	{
		if (answer == 0)
		{
			return readDataOperation::getTeamPlayers;
		}
		if (answer == 1)
		{
			return readDataOperation::getMatchTeams;
		}
		if (answer == 2)
		{
			return readDataOperation::getPlayerTeam;
		}
		return readDataOperation::Invalid;
	}

	operations chooseOperationForWorkWIthDatabase(const int answer)
	{
		if(answer == 0)
		{
			return operations::addData;
		}
		if (answer == 1)
		{
			return operations::readData;
		}
		return operations::Invalid;
	}

	int inputAnswer() noexcept
	{
		int answer = 0;
		if (!(std::cin >> answer))
		{
			std::cout << "Input Error" << std::endl;
			std::cin.clear();
			answer = -1;
		}
		std::cin.ignore();
		return answer;
	}


	void offerUserReadNecessaryDatabaseItems(const footballMatchObjectDataBase& footballMatchDataBase, const linkIdTable& footballMatchLinkTable)
	{
		std::string inputLine;
		bool dialogWindowLive = true;
		while (dialogWindowLive)
		{
			printReadElementSwitchDialogWindow();
			const int answer = inputAnswer();
			switch (chooseOperationToReadDatabase(answer))
			{
			case readDataOperation::getTeamPlayers:
			{
				readTeamPlayers(inputLine, footballMatchDataBase, footballMatchLinkTable);
				break;
			}
			case readDataOperation::getMatchTeams:
			{
				readMatchTeams(inputLine, footballMatchDataBase, footballMatchLinkTable);
				break;
			}
			case readDataOperation::getPlayerTeam:
			{
				readPlayerTeam(inputLine, footballMatchDataBase, footballMatchLinkTable);
				break;
			}
			default:
			{
				dialogWindowLive = false;
				break;
			}
			}
		}
	}
	void offerUserSelectPathAddNewItemsToDatabase(databaseElementLoader& Creator, footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
	{
		Creator.setParserStrategy(parserStrategy::addNewElement);
		std::string inputLine;
		bool dialogWindowLive = true;
		while (dialogWindowLive)
		{
			std::cout << "Do you want to add information from console(0) or from file(1) or exit(any key)?" << std::endl;
			const int answer = inputAnswer();
			switch (chooseOperationToAddNewElementDatabase(answer))
			{
			case addDataOperation::addFromConsole:
			{
				addElementFromString(Creator, inputLine, footballMatchDataBase, footballMatchLinkTable);
				break;
			}
			case addDataOperation::addFromFile:
			{
				addElementFromFile(Creator, inputLine, footballMatchDataBase, footballMatchLinkTable);
				break;
			}
			default:
			{
				dialogWindowLive = false;
				break;
			}
			}
		}
	}

	void offerUserSelectFunctionToWorkWithDatabase(databaseElementLoader& Creator,  footballMatchObjectDataBase& footballMatchDataBase, linkIdTable& footballMatchLinkTable)
	{
		bool dialogWindowLive = true;
		while (dialogWindowLive)
		{
			std::cout << "Do you want to add information(0) or read(1)?" << std::endl;
			const int answer = inputAnswer();
			switch (chooseOperationForWorkWIthDatabase(answer))
			{
			case operations::addData:
			{
				offerUserSelectPathAddNewItemsToDatabase(Creator, footballMatchDataBase, footballMatchLinkTable);
				break;
			}
			case operations::readData:
			{
				offerUserReadNecessaryDatabaseItems(footballMatchDataBase, footballMatchLinkTable);
				break;
			}
			default:
			{
				dialogWindowLive = false;
				break;
			}
			}
		}
	}
}

void execute(const std::string& dataFileName, const std::string& dataSaveFileName)
{
	std::ifstream dataFile(dataFileName);
	if (!dataFile.is_open()) {
		std::cout << "Cannot open file" << std::endl;
		return;
	}
	footballMatchObjectDataBase footballMatchDataBase;
	linkIdTable footballMatchLinkTable;
	databaseElementLoader Creator{ parserStrategy::uploadExisting };
	Creator.addElement(dataFile, footballMatchDataBase, footballMatchLinkTable);
	offerUserSelectFunctionToWorkWithDatabase(Creator, footballMatchDataBase, footballMatchLinkTable);
	saveData(dataFile, dataSaveFileName, footballMatchDataBase, footballMatchLinkTable);
}
