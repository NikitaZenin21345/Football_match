#include "dataBaseSaver.h"
#include "match.h"

namespace 
{
	const std::string matchName = "Match";
	const std::string teamName = "Team";
	const std::string playerName = "Player";
	const std::string separator = "\t";
}

void dataBaseSaver::saveMatches(std::ofstream& dataFile) const
{
	auto match_ = footballDataBase.getMatchesListBeginning();
	while (match_ != footballDataBase.getMatchesListEnd())
	{
		dataFile << matchName << separator;
		dataFile << match_->first.getId() << separator << match_->second->getDate() << separator;
		dataFile << match_->second->getPlace() << separator << match_->second->getResult().getFirstScore()<<
			":" << match_->second->getResult().getSecondtScore() << separator;
		for (const auto& teamIdInMatch : footballLinkTable.getTeamsInMatch(match_->first))
		{
			dataFile << teamIdInMatch.getId() << separator;
		}
		dataFile << std::endl;
		++match_;
	}
}

void dataBaseSaver::saveTeams(std::ofstream& dataFile) const
{
	auto team_ = footballDataBase.getTeamsListBeginning();
	while (team_ != footballDataBase.getTeamsListEnd())
	{
		dataFile << teamName << separator;
		dataFile << team_->first.getId() << separator << team_->second->getName();
		dataFile << std::endl;
		++team_;
	}
}

void dataBaseSaver::savePlayers(std::ofstream& dataFile) const
{
	auto player_ = footballDataBase.getPlayersListBeginning();
	while (player_ != footballDataBase.getPlayersListEnd())
	{
		dataFile << playerName << separator;
		dataFile << player_->first.getId() << separator << player_->second->getName() << separator;
		dataFile << footballLinkTable.getPlayerTeam(player_->first).getId();
		dataFile << std::endl;
		++player_;
	}
}
