#include "pch.h"
#include <crtdbg.h> 
#include <fstream>

#include "Football_match/dataBaseSaver.h"
#include "Football_match/databaseElementLoader.h"


class ElementCreatorTest : public ::testing::Test {
protected:
    virtual void SetUp(void) {

        time = {};
        std::istringstream date_stream(dateStr);
        const char* format = "%Y-%m-%d %H:%M:%S";
        date_stream >> std::get_time(&time, format);
    }
    virtual void TearDown(void) {  }

    std::tm time = {};
    std::string place = "MatchPlace";
    std::string dateStr = "2023-12-01 15:45:30";
    dataBase footballMatchDataBase;
    linkTable footballMatchLinkTable;
     databaseElementLoader Creator{ footballMatchDataBase, footballMatchLinkTable, parserStrategy::addNewElementInDataBase };
};

TEST_F(ElementCreatorTest, OnePlayer)
{
    std::string playerName = "Test1";
    std::string teamName = "TestCommand1";
    std::string newElementName = "Player\t" + playerName + "\t" + teamName;
    auto matchPlayer = player(playerName, playerId(0));
    auto matchTeam = team(teamName);
    std::istringstream stream(newElementName);
    Creator.addElement(parseNextLine(stream));
    const auto newPlayerId = footballMatchDataBase.findId(matchPlayer);
    EXPECT_EQ(*footballMatchDataBase.findElement(newPlayerId), matchPlayer);
    const auto newTeamId = footballMatchLinkTable.getPlayerTeam(newPlayerId);
    EXPECT_EQ(*footballMatchDataBase.findElement(newTeamId), matchTeam);
}

TEST_F(ElementCreatorTest, TwoPlayers)
{
    std::string playerName1 = "Test1";
    std::string playerName2 = "Test2";
    std::string teamName = "TestCommand";
    std::string newElementName1 = "Player\t" + playerName1 + "\t" + teamName;
    std::string newElementName2 = "Player\t" + playerName2 + "\t" + teamName;
    auto matchPlayer1 = player(playerName1);
    auto matchPlayer2 = player(playerName2);
    auto matchTeam = team(teamName);
    std::stringstream stream(newElementName1);
    Creator.addElement(parseNextLine(stream));
    stream.str("");
    stream.clear();
    stream << newElementName2;
    Creator.addElement(parseNextLine(stream));
    const auto newPlayerId1 = footballMatchDataBase.findId(matchPlayer1);
    const auto newPlayerId2 = footballMatchDataBase.findId(matchPlayer2);
    EXPECT_EQ(*footballMatchDataBase.findElement(newPlayerId1), matchPlayer1);
    EXPECT_EQ(*footballMatchDataBase.findElement(newPlayerId2), matchPlayer2);
    const auto newTeamId1 = footballMatchLinkTable.getPlayerTeam(newPlayerId1);
    const auto newTeamId2 = footballMatchLinkTable.getPlayerTeam(newPlayerId2);
    auto teamPlayers = footballMatchLinkTable.getAllPlayersInTeam(newTeamId1);
    for(auto teamPlayer : teamPlayers)
    {
        EXPECT_TRUE(teamPlayer == newPlayerId1 || teamPlayer == newPlayerId2);
    }
    EXPECT_EQ(newTeamId1 , newTeamId2);
    EXPECT_EQ(*footballMatchDataBase.findElement(newTeamId1), matchTeam);
    EXPECT_EQ(*footballMatchDataBase.findElement(newTeamId2), matchTeam);
}

TEST_F(ElementCreatorTest, OneTeam)
{
    std::string teamName = "TestCommand1";
    std::string newElementName = "Team\t"  + teamName;
    std::string playerName1 = "Test1";
    std::string newElementName1 = "Player\t" + playerName1 + "\t" + teamName;
    auto matchPlayer1 = player(playerName1);
    auto matchTeam = team(teamName);
    std::stringstream stream(newElementName);
    Creator.addElement(parseNextLine(stream));
    const auto newTeamId = footballMatchDataBase.findId(matchTeam);
    EXPECT_EQ(*footballMatchDataBase.findElement(newTeamId), matchTeam);
    EXPECT_THROW(auto a = footballMatchLinkTable.getAllPlayersInTeam(newTeamId), noFoundTableElement);
    stream.str("");
    stream.clear();
    stream << newElementName1;
    Creator.addElement(parseNextLine(stream));
    const auto newPlayerId1 = footballMatchDataBase.findId(matchPlayer1);
    auto players = footballMatchLinkTable.getAllPlayersInTeam(newTeamId).begin();
    EXPECT_TRUE(*players == newPlayerId1);
}

TEST_F(ElementCreatorTest, ManyTeams)
{
    size_t size = 100;
	for (size_t i = 0; i < size; ++i)
	{
        std::string teamName = "TestCommand" ;
        std::string playerName1 = "Test" + std::to_string(i);
        std::string newElementName1 = "Player\t" + playerName1 + "\t" + teamName;
        auto matchPlayer1 = player(playerName1);
        auto matchTeam = team(teamName);
        std::stringstream stream(newElementName1);
        Creator.addElement(parseNextLine(stream));
        const auto newTeamId = footballMatchDataBase.findId(matchTeam);
        EXPECT_EQ(*footballMatchDataBase.findElement(newTeamId), matchTeam);
	}
    std::string teamName = "TestCommand";
    auto matchTeam = team(teamName);
    const auto newTeamId = footballMatchDataBase.findId(matchTeam);
    auto players = footballMatchLinkTable.getAllPlayersInTeam(newTeamId);
    EXPECT_EQ(players.size(), size);
    for(auto matchPlayer : players)
    {
        const auto TeamId = footballMatchLinkTable.getPlayerTeam(matchPlayer);
        EXPECT_EQ(TeamId, newTeamId);
    }
}

TEST_F(ElementCreatorTest, OneMatch)
{
    std::string timeStr = dateStr;
    std::string plasceStr = place;
    std::string resultStr = "4:1";
    std::string firstTeamName = "PSG";
    std::string secondTeamName = "Liverpool";
    std::string newElementName = "Match\t" + timeStr + "\t" + plasceStr + "\t" + resultStr + "\t" + firstTeamName + "\t" + secondTeamName;
    auto newMatch = match(time, place, result(4, 1));
    std::istringstream stream(newElementName);
    Creator.addElement(parseNextLine(stream));
    auto firstTeam = team(firstTeamName);
    auto secondTeam = team(secondTeamName);
    const auto newFirstTeamId = footballMatchDataBase.findId(firstTeam);
    EXPECT_EQ(*footballMatchDataBase.findElement(newFirstTeamId), firstTeam);
    const auto newSecondTeamId = footballMatchDataBase.findId(secondTeam);
    EXPECT_EQ(*footballMatchDataBase.findElement(newSecondTeamId), secondTeam);
    auto newMatchId = footballMatchDataBase.findId(newMatch);
    auto teams = footballMatchLinkTable.getTeamsInMatch(newMatchId);
    for (auto team_ : teams)
    {
        EXPECT_TRUE(team_ == newFirstTeamId || team_ == newSecondTeamId);
    }
}

TEST_F(ElementCreatorTest, LoadFromFile)
{
    std::string fileName = "LoadFromFile.txt";
    Creator.setParserStrategy(parserStrategy::loadElementInDataBase);
    std::ifstream dataFile(fileName);
    if (!dataFile.is_open()) {
        std::cerr << "Cannot open file" << std::endl;
        FAIL();
    }
    Creator.addElement(dataFile);
    auto matchBegin = footballMatchDataBase.getMatchesListBeginning();
    EXPECT_EQ(matchBegin->second->getDate(), "Argentina");
}