#include "pch.h"
#include <crtdbg.h> 
#include <fstream>

#include "dataBase.h"
#include "databaseElementLoader.h"
#include "dataBaseSaver.h"
#include "footballMatchObjectCreator.h"



class SaverTest : public ::testing::Test {
protected:
    virtual void SetUp(void) {

    }
    virtual void TearDown(void)
    {  }
    footballMatchObjectDataBase footballMatchDataBase{};
    linkIdTable footballMatchLinkTable{};
    databaseElementLoader Creator{parserStrategy::uploadExisting };
    std::stringstream stream;
    boost::uuids::string_generator gen;
};

TEST_F(SaverTest, saveTeam)
{
    Creator.setParserStrategy(parserStrategy::addNewElement);
    std::string datafile ="Team\tPSG";
    stream << datafile << std::endl;
    Creator.addElement(stream, footballMatchDataBase, footballMatchLinkTable);
    dataBaseSaver saver;
    std::stringstream savestream;
    saver.save(savestream, footballMatchDataBase, footballMatchLinkTable);
    std::string saved;
    savestream >> saved;
    EXPECT_EQ(saved, "Team");
    savestream >> saved;
    auto savedUuid = gen(saved);
    EXPECT_TRUE(footballMatchDataBase.getTeams().begin().operator*().getId().getId() == savedUuid);
    savestream >> saved;
    EXPECT_EQ(saved, "PSG");
}

TEST_F(SaverTest, savePlayer)
{
    Creator.setParserStrategy(parserStrategy::addNewElement);
    std::string datafile = "Player\tNeymar\tPSG";
    stream << datafile << std::endl;
    Creator.addElement(stream, footballMatchDataBase, footballMatchLinkTable);
    dataBaseSaver saver;
    std::stringstream savestream;
    saver.save(savestream, footballMatchDataBase, footballMatchLinkTable);
    std::string saved;
    savestream >> saved;
    EXPECT_EQ(saved, "Team");
    savestream >> saved;
    auto savedTeamUuid = gen(saved);
    EXPECT_TRUE(footballMatchDataBase.getTeams().begin().operator*().getId().getId() == savedTeamUuid);
    savestream >> saved;
    EXPECT_EQ(saved, "PSG");
    savestream >> saved;
    EXPECT_EQ(saved, "Player");
    savestream >> saved;
    auto savedPlayerUuid = gen(saved);
    EXPECT_TRUE(footballMatchDataBase.getPlayers().begin().operator*().getId().getId() == savedPlayerUuid);
    savestream >> saved;
    EXPECT_EQ(saved, "Neymar");
    savestream >> saved;
    auto savedPlayerTeamUuid = gen(saved);
    EXPECT_EQ(savedPlayerTeamUuid, savedTeamUuid);
}

TEST_F(SaverTest, saveMatch)
{
    Creator.setParserStrategy(parserStrategy::addNewElement);
    std::string datafile = "Match\t2023-09-01 15:45:30\tMoscow\t1:1\tPSG\tCSK";
    stream << datafile << std::endl;
    Creator.addElement(stream, footballMatchDataBase, footballMatchLinkTable);
    dataBaseSaver saver;
    std::stringstream savestream;
    saver.save(savestream, footballMatchDataBase, footballMatchLinkTable);
    std::string saved;
    savestream >> saved;
    EXPECT_EQ(saved, "Match");
    savestream >> saved;
    auto savedMatchUuid = gen(saved);
    EXPECT_TRUE(footballMatchDataBase.getMatches().begin().operator*().getId().getId() == savedMatchUuid);
    savestream >> saved;
    EXPECT_EQ(saved, "2023-09-01");
    savestream >> saved;
    EXPECT_EQ(saved,"15:45:30");
    savestream >> saved;
    EXPECT_EQ(saved, "Moscow");
    savestream >> saved;
    EXPECT_EQ(saved, "1:1");
    savestream >> saved;
    auto savedTeamUuid1 = gen(saved);
    EXPECT_TRUE(footballMatchDataBase.findElementById(teamId(savedTeamUuid1)).has_value());
    savestream >> saved;
    auto savedTeamUuid2 = gen(saved);
    EXPECT_TRUE(footballMatchDataBase.findElementById(teamId(savedTeamUuid2)).has_value());
    savestream >> saved;
}


TEST_F(SaverTest, saveString)
{
    std::string datafile = "Match\t56c24847-5355-4562-bb49-1645889fb526\t2023-09-01 15:45:30\tMoscow"
        "\t1:1\tdb28518d-000f-43b7-a841-884ce89f6869\tdd30f9a7-8162-481f-acf8-062713720aee\n"
        "Team\tdd30f9a7-8162-481f-acf8-062713720aee\tKazan-rubin\n"
        "Team\tdb28518d-000f-43b7-a841-884ce89f6869\tPSG\n"
        "Player\t58866766-1219-4320-896a-f3125a864eba\tDiego Maradonna\tdd30f9a7-8162-481f-acf8-062713720aee\n";
    stream << datafile << std::endl;
    Creator.addElement(stream, footballMatchDataBase, footballMatchLinkTable);
    EXPECT_EQ(footballMatchDataBase.getPlayers().size(), 1);
    EXPECT_EQ(footballMatchDataBase.getTeams().size(), 2);
    EXPECT_EQ(footballMatchDataBase.getMatches().size(), 1);
    dataBaseSaver saver;
    std::stringstream savestream;
    saver.save(savestream, footballMatchDataBase, footballMatchLinkTable);
    footballMatchObjectDataBase otherFootballMatchDataBase{};
    linkIdTable otherFootballMatchLinkTable{};
    databaseElementLoader otherCreator{parserStrategy::uploadExisting };
    otherCreator.addElement(savestream, otherFootballMatchDataBase, otherFootballMatchLinkTable);
    EXPECT_EQ(otherFootballMatchDataBase.getPlayers().size(), 1);
    EXPECT_EQ(otherFootballMatchDataBase.getTeams().size(), 2);
    EXPECT_EQ(otherFootballMatchDataBase.getMatches().size(), 1);
}