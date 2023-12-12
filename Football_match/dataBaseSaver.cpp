#include "dataBaseSaver.h"
#include "match.h"

namespace 
{
	const std::string matchName = "Match";
	const std::string teamName = "Team";
	const std::string playerName = "Player";
	const std::string separator = "\t";
}

void dataBaseSaver::saveMatches(std::ostream& dataFile) const
{
	auto listMatch = footballDataBase.getMatches().cbegin();
	while (listMatch != footballDataBase.getMatches().cend())
	{
		dataFile << matchName << separator;
		dataFile << listMatch->first.getId() << separator;
		dataFile << listMatch->second.getDate() << separator;
		dataFile << listMatch->second.getPlace() << separator;
		dataFile << listMatch->second.getResult().getFirstScore() << ":" << listMatch->second.getResult().getSecondScore() << separator;
		for (const auto& teamIdInMatch : footballLinkTable.getTeamsInMatch(listMatch->first))
		{
			dataFile << teamIdInMatch.getId() << separator;
		}
		dataFile << std::endl;
		++listMatch;
	}
}

void dataBaseSaver::saveTeams(std::ostream& dataFile) const
{
	auto listTeam = footballDataBase.getTeams().cbegin();
	while (listTeam != footballDataBase.getTeams().cend())
	{
		dataFile << teamName << separator;
		dataFile << listTeam->first.getId() << separator << listTeam->second.getName();
		dataFile << std::endl;
		++listTeam;
	}
}

void dataBaseSaver::savePlayers(std::ostream& dataFile) const
{
	auto listPlayer = footballDataBase.getPlayers().cbegin();
	while (listPlayer != footballDataBase.getPlayers().cend())
	{
		dataFile << playerName << separator;
		dataFile << listPlayer->first.getId() << separator << listPlayer->second.getName() << separator;
		dataFile << footballLinkTable.getPlayerTeam(listPlayer->first).getId();
		dataFile << std::endl;
		++listPlayer;
	}
}
