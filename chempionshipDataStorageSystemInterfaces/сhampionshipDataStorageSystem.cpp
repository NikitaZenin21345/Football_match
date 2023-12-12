#include <fstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ñhampionshipDataStorageSystem.h"
#include "Football_match/databaseElementLoader.h"
#include "Football_match/elementCreator.h"
#include "Football_match/dataBaseSaver.h"

namespace
{
	enum operations { addData, readData, Invalid };
	enum addDataOperation {  addFromConsole , addFromFile };
	enum readDataOperation { getTeamPlayers = 1, getMatchTeams = 2, getPlayerTeam = 3 };

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
	void printElements(const std::unordered_set<Id, idHash<Id>>& elements, const dataBase& footballMatchDataBase)
	{
		for (const auto& element : elements)
		{
			print<Element>(footballMatchDataBase.findElement(element));
		}
	}

	void saveData(std::ifstream& dataFile, const std::string& dataSaveFileName, dataBase& footballMatchDataBase,
		linkTable& footballMatchLinkTable)
	{
		dataFile.close();
		std::ofstream saveFile(dataSaveFileName);
		if (!saveFile.is_open()) {
			std::cout << "Cannot open file" << std::endl;
			return;
		}
		const dataBaseSaver saver{ footballMatchDataBase, footballMatchLinkTable };
		saver.save(saveFile);
		saveFile.close();
	}

	void addElementFromString(databaseElementLoader& Creator, std::string& inputLine)
	{
		std::getline(std::cin, inputLine);
		std::istringstream stringStream(inputLine);
		auto parser = textFileParser(stringStream);
		Creator.addElement(parser.getParsed());
	}

	void addElementFromFile(databaseElementLoader& Creator, std::string& inputLine)
	{
		std::getline(std::cin, inputLine);
		std::ifstream sourceFile(inputLine);
		if (!sourceFile.is_open())
		{
			std::cerr << "Cannot open file" << std::endl;
			return;
		}
		Creator.addElement(sourceFile);
	}

	void readTeamPlayers(std::string& inputLine,
		const dataBase& footballMatchDataBase, const linkTable& footballMatchLinkTable)
	{
		std::cout << "Input team parameters:" << std::endl;
		std::getline(std::cin, inputLine);
		std::istringstream stringStream(inputLine);
		auto parser = textFileParser(stringStream);
		const auto searchedTeam = elementCreator::createTeamFromToken(parser.getParsed(), 0);
		const auto searchedTeamId = footballMatchDataBase.findId(searchedTeam);
		if (searchedTeamId != teamId())
		{
			printElements<playerId, player>(footballMatchLinkTable.getAllPlayersInTeam(searchedTeamId), footballMatchDataBase);
		}
	}

	void readMatchTeams(std::string& inputLine,
		const dataBase& footballMatchDataBase, const linkTable& footballMatchLinkTable)
	{
		std::cout << "Input Match parameters:" << std::endl;
		std::getline(std::cin, inputLine);
		std::istringstream stringStream(inputLine);
		auto parser = textFileParser(stringStream);
		const auto searchedTeam = elementCreator::createMatchFromToken(parser.getParsed(), 0, 1, 2);
		const auto searchedMatchId = footballMatchDataBase.findId(searchedTeam);
		if (searchedMatchId != matchId())
		{
			printElements<teamId, team>(footballMatchLinkTable.getTeamsInMatch(searchedMatchId), footballMatchDataBase);
		}
	}

	void readPlayerTeam(std::string& inputLine,
		const dataBase& footballMatchDataBase, const linkTable& footballMatchLinkTable)
	{
		std::cout << "Input player parameters:" << std::endl;
		std::getline(std::cin, inputLine);
		std::istringstream stringStream(inputLine);
		auto parser = textFileParser(stringStream);
		const auto searchedPlayer = elementCreator::createPlayerFromToken(parser.getParsed(), 0);
		const auto searchedPLayerId = footballMatchDataBase.findId(searchedPlayer);
		if (searchedPLayerId != playerId())
		{
			std::cout << footballMatchDataBase.findElement(footballMatchLinkTable.getPlayerTeam(searchedPLayerId)).getName() << std::endl;
		}
	}

	void printReadElementSwitchDialogWindow() noexcept
	{
		std::cout << "Switch option\n";
		std::cout << "1.Get a list of all the team's players\n";
		std::cout << "2.Get a match team's \n";
		std::cout << "3.Get a team player`s" << std::endl;
	}

	int inputAnswer() noexcept
	{
		int answer = 0;
		if (!(std::cin >> answer))
		{
			std::cout << "Input Error" << std::endl;
			std::cin.clear();
			answer = Invalid;
		}
		std::cin.ignore();
		return answer;
	}


	void readElementSwitch(const dataBase& footballMatchDataBase, const linkTable& footballMatchLinkTable)
	{
		std::string inputLine;
		bool dialogWindowLive = true;
		while (dialogWindowLive)
		{
			printReadElementSwitchDialogWindow();
			const int answer = inputAnswer();
			if (answer == -1) { continue; }
			switch (answer)
			{
			case getTeamPlayers:
			{
				readTeamPlayers(inputLine, footballMatchDataBase, footballMatchLinkTable);
				break;
			}
			case getMatchTeams:
			{
				readMatchTeams(inputLine, footballMatchDataBase, footballMatchLinkTable);
				break;
			}
			case getPlayerTeam:
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
	void addDataOperationSwitch(databaseElementLoader& Creator)
	{
		Creator.setParserStrategy(parserStrategy::addNewElementInDataBase);
		std::string inputLine;
		bool dialogWindowLive = true;
		while (dialogWindowLive)
		{
			std::cout << "Do you want to add information from console(0) or from file(1) or exit(any key)?\n";
			const int answer = inputAnswer();
			if (answer == -1) { continue; }
			switch (answer)
			{
			case addFromConsole:
			{
				addElementFromString(Creator, inputLine);
				break;
			}
			case addFromFile:
			{
				addElementFromFile(Creator, inputLine);
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

	void launchDialogWindow(databaseElementLoader& Creator,const dataBase& footballMatchDataBase,const linkTable& footballMatchLinkTable)
	{
		bool dialogWindowLive = true;
		while (dialogWindowLive)
		{
			std::cout << "Do you want to add information(0) or read(1)?" << std::endl;
			const int answer = inputAnswer();
			if (answer == Invalid) { continue;}
			switch (answer)
			{
			case addData:
			{
				addDataOperationSwitch(Creator);
				break;
			}
			case readData:
			{
				readElementSwitch(footballMatchDataBase, footballMatchLinkTable);
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
	dataBase footballMatchDataBase;
	linkTable footballMatchLinkTable;
	databaseElementLoader Creator{ footballMatchDataBase, footballMatchLinkTable, parserStrategy::loadElementInDataBase };
	Creator.addElement(dataFile);
	launchDialogWindow(Creator, footballMatchDataBase, footballMatchLinkTable);
	saveData(dataFile, dataSaveFileName, footballMatchDataBase, footballMatchLinkTable);
}
