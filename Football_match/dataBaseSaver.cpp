#include "dataBaseSaver.h"
#include "match.h"

namespace 
{
	const std::string matchName = "Match";
	const std::string teamName = "Team";
	const std::string playerName = "Player";
	const std::string separator = "\t";
}

void dataBaseSaver::saveMatches(std::ostream& dataFile,const footballMatchObjectDataBase& footballDataBase,const linkIdTable& footballLinkTable) const
{
	const auto listMatches = footballDataBase.getMatches();
	for(const auto& oneMatch : listMatches)
	{
		dataFile << matchName << separator;
		dataFile << oneMatch.getId().getId() << separator;
		dataFile << oneMatch.getDate() << separator;
		dataFile << oneMatch.getPlace() << separator;
		dataFile << oneMatch.getResult().getFirstScore() << ":" << oneMatch.getResult().getSecondScore() << separator;
		auto teamsInMatch = footballLinkTable.getTeamsInMatch(oneMatch.getId());
		if(teamsInMatch.has_value())
		{
			for (const auto& teamIdInMatch : teamsInMatch.value())
			{
				dataFile << teamIdInMatch.getId() << separator;
			}
		}
		dataFile << std::endl;
	}
}

void dataBaseSaver::saveTeams(std::ostream& dataFile, const footballMatchObjectDataBase& footballDataBase, [[maybe_unused]]const linkIdTable& footballLinkTable) const
{
	auto listTeam = footballDataBase.getTeams();
	for (const auto oneTeam : listTeam)
	{
		dataFile << teamName << separator;
		dataFile << oneTeam.getId().getId() << separator << oneTeam.getName();
		dataFile << std::endl;
	}
}

void dataBaseSaver::savePlayers(std::ostream& dataFile, const footballMatchObjectDataBase& footballDataBase, const linkIdTable& footballLinkTable) const
{
	auto listPlayer = footballDataBase.getPlayers();
	for (const auto& onePlayer : listPlayer)
	{
		dataFile << playerName << separator;
		dataFile << onePlayer.getId().getId() << separator << onePlayer.getName() << separator;
		dataFile << footballLinkTable.getPlayerTeam(onePlayer.getId()).value().getId();
		dataFile << std::endl;
	}
}
