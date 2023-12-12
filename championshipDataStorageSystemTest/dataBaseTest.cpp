#include "pch.h"
#include <crtdbg.h> 
#include <fstream>
#include "Football_match/dataBaseSaver.h"
#include "Football_match/elementCreator.h"
#include "Football_match/databaseElementLoader.h"


class DataBaseTest : public ::testing::Test {
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
