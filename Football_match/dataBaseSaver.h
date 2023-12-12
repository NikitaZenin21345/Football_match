#pragma once
#include <fstream>
#include "dataBase.h"
#include "linkTable.h"

class dataBaseSaver final
{
	dataBase& footballDataBase;
	linkTable& footballLinkTable;

	void saveMatches(std::ostream& dataFile) const;
	void saveTeams(std::ostream& dataFile) const;
	void savePlayers(std::ostream& dataFile) const;

public:
	dataBaseSaver(dataBase& footballDataBase_, linkTable& footballLinkTable_) :
		footballDataBase(footballDataBase_), footballLinkTable(footballLinkTable_){}

	void save(std::ostream& dataFile) const
	{
		saveMatches(dataFile);
		saveTeams(dataFile);
		savePlayers(dataFile);
	}
};
