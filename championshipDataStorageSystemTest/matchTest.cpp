#include "pch.h"
#include <crtdbg.h> 
#include "footballMatchObjectCreator.h"
#include "match.h"


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
    textFileSeparatedBySimpleDelimitersParser parser(stream);
    stream << newMatchName;
	const auto testMatch = footballMatchObjectCreator::createMatchFromToken(parser.getParsed(), 0, 1, 2);
    EXPECT_EQ(testMatch.getResult() , result(3,1));
    EXPECT_EQ(testMatch.getDate(), dateStr);
    EXPECT_EQ(testMatch.getPlace() , placeName);
}

TEST_F(MatchTest, Result)
{
    textFileSeparatedBySimpleDelimitersParser parser(stream, ":");
    stream << resultName;
    auto parsedString = parser.getParsed();
    EXPECT_EQ(std::stoi(parsedString.getTokens(0)), 3);
    EXPECT_EQ(std::stoi(parsedString.getTokens(1)), 1);
    std::string badResultName = "-3:";
    stream.clear();
    stream << resultName;
	parsedString = parser.getParsed();
}

TEST_F(MatchTest, Data)
{
    textFileSeparatedBySimpleDelimitersParser parser(stream);
    stream << dateStr;
    date matchData(date::getTime(parser.getParsed().getTokens(0)));
    EXPECT_EQ(matchData.getDataInString(), dateStr);
    const auto badDate = date::getTime("15:45:30");
    EXPECT_TRUE(badDate.tm_year == 0 && badDate.tm_mon == 0 && badDate.tm_mday == 0 &&
        badDate.tm_hour == 0 &&
        badDate.tm_min == 0 &&
        badDate.tm_sec == 0 );
}