#include "pch.h"
#include <crtdbg.h> 
#include <fstream>

#include "Football_match/databaseElementLoader.h"
#include "Football_match/dataBaseSaver.h"
#include "Football_match/elementCreator.h"
#include "Football_match/dataBaseSaver.h"


class SaverTest : public ::testing::Test {
protected:
    virtual void SetUp(void) {

    }
    virtual void TearDown(void)
    {  }
    dataBase footballMatchDataBase{};
    linkTable footballMatchLinkTable{};
    databaseElementLoader Creator{ footballMatchDataBase, footballMatchLinkTable, parserStrategy::loadElementInDataBase };
    std::stringstream stream;
    
};

TEST_F(SaverTest, savePLayer)
{
    Creator.setParserStrategy(parserStrategy::addNewElementInDataBase);
    std::string datafile ="Team\tPSG";
    stream << datafile << std::endl;
    Creator.addElement(stream);
    dataBaseSaver saver(footballMatchDataBase, footballMatchLinkTable);
    std::stringstream savestream;
    saver.save(savestream);
    std::string saved;
    savestream >> saved;
    EXPECT_EQ(saved, "Team");
    savestream >> saved;
    savestream >> saved;
    EXPECT_EQ(saved, "PSG");
}

TEST_F(SaverTest, saveString)
{
    std::string datafile = "Match\t56c24847-5355-4562-bb49-1645889fb526\t2023-09-01 15:45:30\tMoscow"
        "\t1:1\tdb28518d-000f-43b7-a841-884ce89f6869\tdd30f9a7-8162-481f-acf8-062713720aee\n"
        "Team\tdd30f9a7-8162-481f-acf8-062713720aee\tKazan-rubin\n"
        "Team\tdb28518d-000f-43b7-a841-884ce89f6869\tPSG\n"
        "Player\t58866766-1219-4320-896a-f3125a864eba\tDiego Maradonna\tdd30f9a7-8162-481f-acf8-062713720aee\n";
    stream << datafile << std::endl;
    Creator.addElement(stream);
    EXPECT_TRUE(footballMatchDataBase.getPlayers().size() == 1);
    EXPECT_TRUE(footballMatchDataBase.getTeams().size() == 2);
    EXPECT_TRUE(footballMatchDataBase.getMatches().size() == 1);
    dataBaseSaver saver(footballMatchDataBase, footballMatchLinkTable);
    std::stringstream savestream;
    saver.save(savestream);
    dataBase otherFootballMatchDataBase{};
    linkTable otherFootballMatchLinkTable{};
    databaseElementLoader otherCreator{ otherFootballMatchDataBase, otherFootballMatchLinkTable, parserStrategy::loadElementInDataBase };
    otherCreator.addElement(savestream);
    EXPECT_TRUE(otherFootballMatchDataBase.getPlayers().size() == 1);
    EXPECT_TRUE(otherFootballMatchDataBase.getTeams().size() == 2);
    EXPECT_TRUE(otherFootballMatchDataBase.getMatches().size() == 1);
}