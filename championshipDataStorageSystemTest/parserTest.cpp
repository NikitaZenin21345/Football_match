#include "pch.h"

#include "parser.h"


class parserTests : public ::testing::Test {
protected:
    virtual void SetUp(void) {

    }
    virtual void TearDown(void)
    {  }
    std::stringstream stream;

};

TEST_F(parserTests,parserString)
{
    const std::string teamName = "TestTeam1";
    const std::string playName = "Player";
    const std::string testName = "Test1";
    const std::string newElementName = playName + "\t" + testName + "\t" + teamName;
    stream << newElementName;
    textFileSeparatedBySimpleDelimitersParser parser(stream);
    auto parsedString = parser.getParsed();
    EXPECT_EQ(parsedString.getTokens(0), playName);
    EXPECT_EQ(parsedString.getTokens(1), testName);
    EXPECT_EQ(parsedString.getTokens(2), teamName);
    stream.clear();
    stream << "";
    EXPECT_EQ(parser.getParsed().isValid(), false);
}

TEST_F(parserTests, parseStream)
{
    size_t size = 100;
	for (int i = 0; i < size; ++i)
	{
        std::string newElementName = std::to_string(i) + "\t" + std::to_string(i) + " " + std::to_string(i);
        stream << newElementName << std::endl;
	}
    textFileSeparatedBySimpleDelimitersParser parser(stream, "\t ");
    for (size_t i = 0; i <  size/3; ++i)
    {
        auto parsedStream = parser.getParsed();
        EXPECT_EQ(parsedStream.isValid(), true);
        EXPECT_EQ(parsedStream.getTokens(0), std::to_string(i));
        EXPECT_EQ(parsedStream.getTokens(1), std::to_string(i));
        EXPECT_EQ(parsedStream.getTokens(2), std::to_string(i));
    }
}

TEST_F(parserTests, parserExceptions)
{
    const std::string teamName = "TestTeam1";
    const std::string playName = "Player";
    const std::string testName = "Test1";
    const std::string newElementName = playName + "\t" + testName + "\t" + teamName;
    stream << newElementName;
    textFileSeparatedBySimpleDelimitersParser parser(stream);
    auto parsedString = parser.getParsed();
    EXPECT_THROW(auto noUsed = parsedString.getTokens(4), std::out_of_range);
}