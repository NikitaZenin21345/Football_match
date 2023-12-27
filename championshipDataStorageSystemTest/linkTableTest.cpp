#include "pch.h"
#include <crtdbg.h> 
#include <fstream>
#include "dataBaseSaver.h"
#include "footballMatchObjectCreator.h"
#include "databaseElementLoader.h"


class LinkTableTest : public ::testing::Test {
protected:
    virtual void SetUp(void) {

    }
    virtual void TearDown(void)
    {  }
    footballMatchObjectDataBase footballMatchDataBase{};
    linkIdTable footballMatchLinkTable{};
    databaseElementLoader Creator{ parserStrategy::addNewElement };
    std::stringstream stream;

};

TEST_F(LinkTableTest, onePlayer)
{
    std::string MatchName = "Match\t2003-07-01 20:15:30\tArgentina\t3:1\tPSG\tCSK\tAhmat";
    stream << MatchName;
    Creator.addElement(stream, footballMatchDataBase, footballMatchLinkTable);
    auto matchid = footballMatchDataBase.getMatches().begin().operator*().getId();
    auto firstTeamId = *footballMatchLinkTable.getTeamsInMatch(matchid)->begin();
    auto secondTeamId = *(++footballMatchLinkTable.getTeamsInMatch(matchid)->begin());
    EXPECT_TRUE(*footballMatchLinkTable.getTeamsInMatch(matchid)->begin() == firstTeamId ||
				*(footballMatchLinkTable.getTeamsInMatch(matchid)->begin()) == secondTeamId);
    EXPECT_TRUE(*++footballMatchLinkTable.getTeamsInMatch(matchid)->begin() == firstTeamId ||
        *(++footballMatchLinkTable.getTeamsInMatch(matchid)->begin()) == secondTeamId);
}

TEST_F(LinkTableTest, oneTeam)
{
    auto TeamId = teamId(footballMatchDataBase.getNewId());
    auto PlayerId = playerId(footballMatchDataBase.getNewId());
    footballMatchLinkTable.add(TeamId, PlayerId);
    auto foundTeamId = footballMatchLinkTable.getPlayerTeam(PlayerId);
    EXPECT_EQ(foundTeamId, TeamId);
}