#include "TestCommon.hpp"
#include "../src/control/GameControl.hpp"
#include "../src/control/AlienControl.hpp"
#include "../src/control/PlayerControl.hpp"
#include "../src/control/AlienBulletControl.hpp"
#include "gtest/gtest.h"

int main2(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}