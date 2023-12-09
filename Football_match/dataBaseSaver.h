#pragma once
#include <fstream>
#include "dataBase.h"
#include "linkTable.h"

class dataBaseSaver final
{
	dataBase& footballDataBase;
	linkTable& footballLinkTable;

	void saveMatches(std::ofstream& dataFile) const;
	void saveTeams(std::ofstream& dataFile) const;
	void savePlayers(std::ofstream& dataFile) const;

public:
	dataBaseSaver(dataBase& footballDataBase_, linkTable& footballLinkTable_) :
		footballDataBase(footballDataBase_), footballLinkTable(footballLinkTable_){}

	void save(std::ofstream& dataFile) const
	{
		saveMatches(dataFile);
		saveTeams(dataFile);
		savePlayers(dataFile);
	}
};
