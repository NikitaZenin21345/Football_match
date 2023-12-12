#include "pch.h"
#include <crtdbg.h> 
#include "Football_match/elementCreator.h"
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
    std::string resultName = "3:1";
    std::string placeName = "MatchPlace";
    std::string dateStr = "2023-12-01 15:45:30";
    std::string newMatchName = dateStr + "\t" + placeName + "\t" + resultName;
    std::stringstream stream;
};

TEST_F(MatchTest, Constructor)
{
    textFileParser parser(stream);
    stream << newMatchName;
	const auto testMatch = elementCreator::createMatchFromToken(parser.getParsed(), 0, 1, 2);
    EXPECT_EQ(testMatch.getResult() , result(3,1));
    EXPECT_EQ(testMatch.getDate(), dateStr);
    EXPECT_EQ(testMatch.getPlace() , placeName);
}

TEST_F(MatchTest, Result)
{
    textFileParser parser(stream, ":");
    stream << resultName;
    auto parsedString = parser.getParsed();
    EXPECT_EQ(std::stoi(parsedString.getTokens(0)), 3);
    EXPECT_EQ(std::stoi(parsedString.getTokens(1)), 1);
    std::string badResultName = "-3:";
    stream.clear();
    stream << resultName;
	parsedString = parser.getParsed();
}
   