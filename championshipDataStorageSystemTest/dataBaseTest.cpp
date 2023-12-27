#include "pch.h"
#include <crtdbg.h> 
#include <fstream>

#include "dataBase.h"
#include "footballMatchObjectCreator.h"
#include "databaseElementLoader.h"


class DataBaseTest : public ::testing::Test {
protected:
    virtual void SetUp(void) {

    }
    virtual void TearDown(void)
    {  }
    footballMatchObjectDataBase footballMatchDataBase{};
    linkIdTable footballMatchLinkTable{};
    databaseElementLoader Creator{ parserStrategy::addNewElement };
    std::stringstream stream;
    boost::uuids::string_generator gen;
};


TEST_F(DataBaseTest, OnePLayer)
{
    const auto newPLayerId = playerId(footballMatchDataBase.getNewId());
    std::string playerName = "Neymar";
    auto newPlayer = player(playerName, newPLayerId);
    footballMatchDataBase.add(newPLayerId, newPlayer);
    EXPECT_EQ(footballMatchDataBase.findId(newPlayer), newPLayerId);
    EXPECT_EQ(footballMatchDataBase.findElementById(newPLayerId), newPlayer);
    const auto noFoundId = playerId(footballMatchDataBase.getNewId());
    std::string noFoundPlayerName = "Linonel Messi";
    auto noFoundTeam = player(noFoundPlayerName, noFoundId);
    EXPECT_NO_THROW(auto noUsed = footballMatchDataBase.findElementById(noFoundId));
}

TEST_F(DataBaseTest, OneTeam)
{
    const auto newTeamId = teamId(footballMatchDataBase.getNewId());
    std::string TeamName = "PSG";
    auto newTeam= team(TeamName, newTeamId);
    footballMatchDataBase.add(newTeamId, newTeam);
    EXPECT_EQ(footballMatchDataBase.findId(newTeam), newTeamId);
    EXPECT_EQ(footballMatchDataBase.findElementById(newTeamId), newTeam);
    const auto noFoundId = teamId(footballMatchDataBase.getNewId());
    std::string noFoundTeamName = "PSG1";
    auto noFoundTeam = team(noFoundTeamName, noFoundId);
    EXPECT_NO_THROW(auto noUsed = footballMatchDataBase.findElementById(noFoundId));
}

TEST_F(DataBaseTest, OneMatch)
{
    textFileSeparatedBySimpleDelimitersParser parser(stream);
    const auto newMatchId = matchId(footballMatchDataBase.getNewId());
    const std::string MatchName = "Match\t2003-07-01 20:15:30\tArgentina\t3:1\tPSG\tCSK\tAhmat";
    stream << MatchName;
    auto testMatch = footballMatchObjectCreator::createMatchFromToken(parser.getParsed(), 1, 2, 3);
    footballMatchDataBase.add(newMatchId, testMatch);
    EXPECT_EQ(footballMatchDataBase.findId(testMatch), newMatchId);
    EXPECT_EQ(footballMatchDataBase.findElementById(newMatchId), testMatch);
}

TEST_F(DataBaseTest, OneMatchThreeTeems)
{
    textFileSeparatedBySimpleDelimitersParser parser(stream);
    auto firstTeamId = teamId(footballMatchDataBase.getNewId());
    auto secondTeamId = teamId(footballMatchDataBase.getNewId());
    auto thirdTeamId = teamId(footballMatchDataBase.getNewId());
    std::string firstTeamName = "PSG";
    std::string secondTeamName = "CSK";
    std::string thirdTeamName = "Ahmat";
    auto firstTeam = team(firstTeamName, firstTeamId);
    auto secondTeam = team(secondTeamName, secondTeamId);
    auto thirdTeam = team(thirdTeamName, thirdTeamId);
    footballMatchDataBase.add(firstTeamId, firstTeam);
    footballMatchDataBase.add(secondTeamId, secondTeam);
    footballMatchDataBase.add(thirdTeamId, thirdTeam);
    EXPECT_EQ(footballMatchDataBase.getTeams().size(), 3);
    std::string MatchName = "Match\t2003-07-01 20:15:30\tArgentina\t3:1\tPSG\tCSK\tAhmat";
	stream << MatchName;
    Creator.addElement(stream, footballMatchDataBase, footballMatchLinkTable);
    EXPECT_EQ(footballMatchDataBase.getMatches().size(), 1);
    EXPECT_TRUE((*footballMatchDataBase.getMatches().begin()).getPlace() == "Argentina");
    EXPECT_TRUE((*footballMatchDataBase.getMatches().begin()).getResult() == result(3,1));
    EXPECT_TRUE((*footballMatchDataBase.getMatches().begin()).getDate() == "2003-07-01 20:15:30");
    auto firstFoundTeam = *footballMatchLinkTable.getTeamsInMatch((*footballMatchDataBase.getMatches().begin()).getId())->begin();
    auto secondFoundTeam = *(++(footballMatchLinkTable.getTeamsInMatch((*footballMatchDataBase.getMatches().begin()).getId()).value().begin()));
    EXPECT_TRUE(firstFoundTeam == firstTeamId || firstFoundTeam == secondTeamId );
    EXPECT_TRUE(secondFoundTeam == firstTeamId || secondFoundTeam == secondTeamId );
}