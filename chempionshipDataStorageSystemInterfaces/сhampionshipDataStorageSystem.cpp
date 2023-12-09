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
	enum addDataOperation { addData, readData, addFromConsole, addFromFile };
	enum readDataOperation { getListAllTeamsPlayer, getMatchTeams, getPlayersCommand };

	void saveData(std::ifstream& dataFile, const std::string& dataSaveFileName, dataBase& footballMatchDataBase,
		linkTable& footballMatchLinkTable)
	{
		dataFile.close();
		std::ofstream saveFile(dataSaveFileName);
		dataBaseSaver saver{ footballMatchDataBase, footballMatchLinkTable };
		saver.save(saveFile);
		saveFile.close();
	}

	void addElementFromConsole(databaseElementLoader& Creator, std::string& inputLine)
	{
		std::getline(std::cin, inputLine);
		std::istringstream stringStream(inputLine);
		Creator.addElement(parseNextLine(stringStream));
	}

	void addElementFromFile(databaseElementLoader& Creator, std::string& inputLine)
	{
		std::getline(std::cin, inputLine);
		std::istringstream stringStream(inputLine);
		std::ifstream sourceFile(parseNextLine(stringStream, "").getTokens(0));
		if (!sourceFile.is_open()) {
			std::cerr << "Cannot open file" << std::endl;
			return;
		}
		Creator.addElement(sourceFile);
	}

	void readTeamsPlayers(databaseElementLoader& Creator, std::string& inputLine,
		const dataBase& footballMatchDataBase, const linkTable& footballMatchLinkTable)
	{
		std::getline(std::cin, inputLine);
		std::istringstream stringStream(inputLine);
		auto token = parseNextLine(stringStream);
		auto foundTeam = elementCreator::createTeamFromToken(token, 1);
		auto team = footballMatchDataBase.getTeamsListBeginning();
		while (team != footballMatchDataBase.getTeamsListEnd())
		{
			if (*team->second == foundTeam)
			{

			}
		}
		//footballMatchLinkTable.getAllPlayersInTeam();
	}

	void readElementSwitch(databaseElementLoader& Creator, int answer, const dataBase& footballMatchDataBase, const linkTable& footballMatchLinkTable)
	{
		std::string inputLine;
		bool dialogWindowLive = true;
		while (dialogWindowLive)
		{
			std::cout << "Switch option\n";
			std::cout << "1.Get a list of all the team's players\n";
			std::cout << "2.Get a match team's \n";
			std::cout << "3.Get a players command\n";
			std::cin >> answer;
			std::cin.ignore();
			switch (answer)
			{
			case getListAllTeamsPlayer:
			{
				readTeamsPlayers(Creator, inputLine, footballMatchDataBase, footballMatchLinkTable);
				break;
			}
			case getMatchTeams:
			{

				break;
			}
			case getPlayersCommand:
			{

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
	void addDataOperationSwitch(databaseElementLoader& Creator, int answer)
	{
		Creator.setParserStrategy(parserStrategy::addNewElementInDataBase);
		std::string inputLine;
		bool dialogWindowLive = true;
		while (dialogWindowLive)
		{
			std::cout << "Do you want to add information from console(2) or from file(3) or exit(any key)?\n";
			std::cin >> answer;
			std::cin.ignore();
			switch (answer)
			{
			case addFromConsole:
			{
				addElementFromConsole(Creator, inputLine);
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

	void launchDialogWindow(databaseElementLoader& Creator, dataBase& footballMatchDataBase, linkTable& footballMatchLinkTable)
	{
		int answer = 0;
		std::cout << "Do you want to add information(0) or read(1)?" << std::endl;
		std::cin >> answer;
		switch (answer)
		{
		case addData:
		{
			addDataOperationSwitch(Creator, answer);
			break;
		}
		case readData:
		{
			readElementSwitch(Creator, answer, footballMatchDataBase, footballMatchLinkTable);
			break;
		}
		}
	}
}

void execute(const std::string& dataFileName, const std::string& dataSaveFileName)
{
	std::ifstream dataFile(dataFileName);
	if (!dataFile.is_open()) {
		std::cerr << "Cannot open file" << std::endl;
		return;
	}
	dataBase footballMatchDataBase;
	linkTable footballMatchLinkTable;
	databaseElementLoader Creator{ footballMatchDataBase, footballMatchLinkTable, parserStrategy::loadElementInDataBase };
	Creator.addElement(dataFile);
	launchDialogWindow(Creator, footballMatchDataBase, footballMatchLinkTable);
	saveData(dataFile, dataSaveFileName, footballMatchDataBase, footballMatchLinkTable);
}
