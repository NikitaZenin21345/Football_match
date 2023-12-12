#include "pch.h"
#include <crtdbg.h> 
#include <fstream>
#include "Football_match/dataBaseSaver.h"
#include "Football_match/elementCreator.h"
#include "Football_match/databaseElementLoader.h"


class ElementCreatorTest : public ::testing::Test {
protected:
    virtual void SetUp(void) {

    }
    virtual void TearDown(void)
    {  }
    dataBase footballMatchDataBase{};
    linkTable footballMatchLinkTable{};
    databaseElementLoader Creator{ footballMatchDataBase, footballMatchLinkTable, parserStrategy::addNewElementInDataBase };
    std::stringstream stream;

};

match addMatchFromStream(std::stringstream& stream,const std::string& newElementName, textFileParser& parser,
		const size_t firstIndex, const size_t secondIndex, const size_t thirdIndex)
{
    stream << newElementName;
    auto newMatch = elementCreator::createMatchFromToken(parser.getParsed(), firstIndex, secondIndex, thirdIndex);
    stream.clear();
    return newMatch;
}

player addPlayerFromStream(std::stringstream& stream, const std::string& newElementName, textFileParser& parser, const size_t index)
{
    stream << newElementName;
    auto newPlayer = elementCreator::createPlayerFromToken(parser.getParsed(), index);
    stream.clear();
    return  newPlayer;
}
team addTeamFromStream(std::stringstream& stream, const std::string& newElementName, textFileParser& parser,const size_t index)
{
    stream << newElementName;
    auto newTeam = elementCreator::createTeamFromToken(parser.getParsed(), index);
    stream.clear();
    return newTeam;
}

TEST_F(ElementCreatorTest, OnePlayer)
{

    const std::string teamName = "TestTeam1";
    const std::string newElementName = "Player\tTest1\t" + teamName;
    stream << newElementName;
    textFileParser parser(stream);
    const auto matchPlayer = elementCreator::createPlayerFromToken(parser.getParsed(), 1);
    stream.seekg(0);
    std::string newTeamName = "Team\t" + teamName;
    Creator.addElement(stream);
    stream.clear();
    stream << newTeamName;
    const auto newPlayerId = footballMatchDataBase.findId(matchPlayer);
    auto matchTeam = elementCreator::createTeamFromToken(parser.getParsed(), 1);
    EXPECT_EQ(footballMatchDataBase.findElement(newPlayerId), matchPlayer);
    const auto newTeamId = footballMatchLinkTable.getPlayerTeam(newPlayerId);
    EXPECT_EQ(footballMatchDataBase.findElement(newTeamId), matchTeam);
}

TEST_F(ElementCreatorTest, TwoPlayers)
{
    textFileParser parser(stream);
    std::string teamName = "TestCommand";
    std::string newElementName1 = "Player\tTest1\t" + teamName;
    std::string newElementName2 = "Player\tTest2\t" + teamName;
    stream << newElementName1;
    Creator.addElement(parser.getParsed());
    stream.clear();
    stream << newElementName2;
    Creator.addElement(parser.getParsed());
    stream.clear();
    auto matchTeam = addTeamFromStream(stream, "Team\t" + teamName, parser, 1);
    auto matchPlayer1 = addPlayerFromStream(stream, newElementName1, parser, 1);
    stream << newElementName2;
    auto matchPlayer2 = elementCreator::createPlayerFromToken(parser.getParsed(), 1);
    const auto newPlayerId1 = footballMatchDataBase.findId(matchPlayer1);
    const auto newPlayerId2 = footballMatchDataBase.findId(matchPlayer2);
    EXPECT_EQ(footballMatchDataBase.findElement(newPlayerId1), matchPlayer1);
    EXPECT_EQ(footballMatchDataBase.findElement(newPlayerId2), matchPlayer2);
    const auto newTeamId1 = footballMatchLinkTable.getPlayerTeam(newPlayerId1);
    const auto newTeamId2 = footballMatchLinkTable.getPlayerTeam(newPlayerId2);
    auto teamPlayers = footballMatchLinkTable.getAllPlayersInTeam(newTeamId1);
    for(auto teamPlayer : teamPlayers)
    {
        EXPECT_TRUE(teamPlayer == newPlayerId1 || teamPlayer == newPlayerId2);
    }
    EXPECT_EQ(newTeamId1 , newTeamId2);
    EXPECT_EQ(footballMatchDataBase.findElement(newTeamId1), matchTeam);
    EXPECT_EQ(footballMatchDataBase.findElement(newTeamId2), matchTeam);
}

TEST_F(ElementCreatorTest, OneTeam)
{
    textFileParser parser(stream);
    std::string teamName = "TestCommand1";
    std::string newElementName = "Team\t"  + teamName;
    std::string playerName1 = "Test1";
    std::string newElementName1 = "Player\t" + playerName1 + "\t" + teamName;
    stream << "Team\t" + teamName;
    Creator.addElement(parser.getParsed());
    stream.clear();
    stream << "Team\t" + teamName;
    auto matchTeam = elementCreator::createTeamFromToken(parser.getParsed(), 1);
    stream.clear();
    stream << newElementName1;
    const auto newTeamId = footballMatchDataBase.findId(matchTeam);
    EXPECT_EQ(footballMatchDataBase.findElement(newTeamId), matchTeam);
    EXPECT_THROW(auto noUsed = footballMatchLinkTable.getAllPlayersInTeam(newTeamId), noFoundTableElement);
    stream << newElementName1;
    auto matchPlayer = elementCreator::createPlayerFromToken(parser.getParsed(), 1);
    stream.clear();
    stream << newElementName1;
    Creator.addElement(parser.getParsed());
    const auto matchPlayerId = footballMatchDataBase.findId(matchPlayer);
    auto players = footballMatchLinkTable.getAllPlayersInTeam(newTeamId).begin();
    EXPECT_TRUE(*players == matchPlayerId);
}

TEST_F(ElementCreatorTest, ManyTeams)
{
    textFileParser parser(stream);
    size_t size = 100;
    const std::string teamName = "TestCommand" ;
	for (size_t i = 0; i < size; ++i)
	{
        std::string playerName1 = "Test" + std::to_string(i);
        std::string newElementName1 = "Player\t" + playerName1 + "\t" + teamName;
        stream << newElementName1;
        Creator.addElement(parser.getParsed());
        stream.clear();
	}
    stream << "Team\t" + teamName;
    auto matchTeam = elementCreator::createTeamFromToken(parser.getParsed(), 1);
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
    textFileParser parser(stream);
    std::string timeStr = "2023-12-01 15:45:30";
    std::string place = "MatchPlace";
    std::string resultStr = "4:1";
    std::string firstTeamName = "PSG";
    std::string secondTeamName = "Liverpool";
    std::string newElementName = "Match\t" + timeStr + "\t" + place + "\t" + resultStr + "\t" + firstTeamName + "\t" + secondTeamName;
    stream << newElementName;
    Creator.addElement(parser.getParsed());
    stream.clear();
    auto newMatch = addMatchFromStream(stream, newElementName, parser, 1, 2, 3);
    auto firstTeam = addTeamFromStream(stream, firstTeamName, parser, 0);
    auto secondTeam = addTeamFromStream(stream, secondTeamName, parser, 0);
    const auto newFirstTeamId = footballMatchDataBase.findId(firstTeam);
    EXPECT_EQ(footballMatchDataBase.findElement(newFirstTeamId), firstTeam);
    const auto newSecondTeamId = footballMatchDataBase.findId(secondTeam);
    EXPECT_EQ(footballMatchDataBase.findElement(newSecondTeamId), secondTeam);
    auto newMatchId = footballMatchDataBase.findId(newMatch);
    auto teams = footballMatchLinkTable.getTeamsInMatch(newMatchId);
    for (auto team_ : teams)
    {
        EXPECT_TRUE(team_ == newFirstTeamId || team_ == newSecondTeamId);
    }
}

TEST_F(ElementCreatorTest, LoadFromFile)
{
    textFileParser parser(stream);
    std::string MatchName = "Match\t2003 - 01 - 01 20:15 : 30\tArgentina\t3:1";
    std::string PlayerName = "Player\tLionel Pepsi";
    std::string teamName = "Team\tCSK";
    std::string fileName = "LoadFromFile.txt";
    std::ofstream dataFile(fileName);
    if (!dataFile.is_open()) {
        std::cout << "Cannot open file" << std::endl;
        FAIL();
    }
    dataFile << MatchName + "\tAhmat\tCSK\nTeam\tAhmat\n"+ teamName+ "\n"+ PlayerName + "\tCSK" << std::endl;
    dataFile.close();
    std::ifstream dataFile1(fileName);
    if (!dataFile1.is_open()) {
        std::cout << "Cannot open file" << std::endl;
        FAIL();
    }
    Creator.addElement(dataFile1);
    auto matchBegin = footballMatchDataBase.getMatches();
    stream.clear();
    stream << MatchName;
    const auto newMatch = elementCreator::createMatchFromToken(parser.getParsed(), 1, 2, 3);
    const auto newMatchId = footballMatchDataBase.findId(newMatch);
    EXPECT_EQ(footballMatchDataBase.findElement(newMatchId), newMatch);
    stream.clear();
    stream << PlayerName;
    auto newPlayer = elementCreator::createPlayerFromToken(parser.getParsed(), 1);
    const auto newPlayerId = footballMatchDataBase.findId(newPlayer);
    stream.clear();
    stream << teamName;
    auto newTeam = elementCreator::createTeamFromToken(parser.getParsed(), 1);
    const auto newTeamId = footballMatchDataBase.findId(newTeam);
    EXPECT_EQ(footballMatchLinkTable.getPlayerTeam(newPlayerId), newTeamId);
}