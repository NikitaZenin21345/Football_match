#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include "elementCreator.h"
#include "dataBaseSaver.h"
int main(int argc, char* argv[])
{
	std::ifstream addFile("input.txt");
	std::ifstream dataFile("example.txt.txt");
	if (!dataFile.is_open()) {
		std::cerr << "" << std::endl;
		return 1;
	}
	dataBase footbalMatchDataBase;
	linkTable footballMatchLinkTable;
	elementCreator Creator{ footbalMatchDataBase, footballMatchLinkTable, parserStrategy::loadNewElementInDataBase };
	Creator.addElement(dataFile);

	int answer = 0;
	std::cout << "Do you want to add information(1) or read(2)?" << std::endl;
	std::cin >> answer;
	switch (answer)
	{
	case 1:
	{
		Creator.setStrategy(parserStrategy::addNewElementInDataBase);
		std::cout << "Do you want to add information from console(1) or from file(2)?\n";
		std::cin >> answer;
		switch (answer)
		{
		case 1:
		{
			std::cin.ignore();
			std::string inputLine;
			std::getline(std::cin, inputLine);
			std::istringstream stringStream(inputLine);
			Creator.addElement(parse_next_line(stringStream));
			break;
		}
		case 2:
		{
			break;
		}
		}
	}
	case 2:
	{
		break;
	}
	}
	dataFile.close();
	std::ofstream saveFile("example.txt.txt"/*, std::ios::trunc*/);
	dataBaseSaver saver{ footbalMatchDataBase, footballMatchLinkTable };
	saver.save(saveFile);
	saveFile.close();
	return 0;
}
