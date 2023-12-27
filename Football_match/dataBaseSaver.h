#pragma once
#include <fstream>
#include "dataBase.h"
#include "linkTable.h"

class dataBaseSaver final
{
	void saveMatches(std::ostream& dataFile,const footballMatchObjectDataBase& footballMatchDataBase, const linkIdTable& footballMatchLinkTable) const;
	void saveTeams(std::ostream& dataFile, const footballMatchObjectDataBase& footballMatchDataBase, const linkIdTable& footballMatchLinkTable) const;
	void savePlayers(std::ostream& dataFile, const footballMatchObjectDataBase& footballMatchDataBase, const linkIdTable& footballMatchLinkTable) const;

public:

	void save(std::ostream& dataFile, const footballMatchObjectDataBase& footballMatchDataBase, const linkIdTable& footballMatchLinkTable) const
	{
		saveMatches(dataFile, footballMatchDataBase, footballMatchLinkTable);
		saveTeams(dataFile, footballMatchDataBase, footballMatchLinkTable);
		savePlayers(dataFile, footballMatchDataBase, footballMatchLinkTable);
	}
};
