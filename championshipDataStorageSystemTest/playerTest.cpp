#include "pch.h"
#include <crtdbg.h> 
#include "Football_match/player.h"

TEST(PlayerTest, Constructor)
{
    std::string name = "TESTNAME";
    auto testPlayer = player(name,  playerId(0));
    EXPECT_EQ(testPlayer.getName(), "TESTNAME");
}