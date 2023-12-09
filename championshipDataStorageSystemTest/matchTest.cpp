#include "pch.h"
#include <crtdbg.h> 

#include "Football_match/match.h"


class MatchTest : public ::testing::Test {
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
};

TEST_F(MatchTest, Constructor)
{
    auto matchPlace = place;
	auto testMatch = match(time, place, result(3, 1));
    EXPECT_EQ(testMatch.getResult() , result(3,1));
    EXPECT_EQ(testMatch.getDate(), dateStr);
    EXPECT_EQ(testMatch.getPlace() , matchPlace);
}