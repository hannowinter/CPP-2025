#include "TestCommon.hpp"
#include "../src/view/BackgroundView.hpp"
#include <numbers>

// Tests whether the angle is always greater than -180 degrees and less than 180 degrees.
TEST(BackgroundTest, backgroundResetDirectionTest)
{
	BackgroundView background;
	std::mt19937 random;
	random.seed(0); // make it deterministic

	for (size_t i = 0; i < 100; i++)
	{
		background.reset_direction(random);
		EXPECT_GE(background.get_direction(), -std::numbers::pi_v<float>);
		EXPECT_LE(background.get_direction(), std::numbers::pi_v<float>);
	}
}

// Tests whether the offset of the top-leftmost tile always stays within [-TILE_SIZE, 0) when moving.
TEST(BackgroundTest, backgroundUpdateTest)
{
	BackgroundView background;
	std::mt19937 random;
	random.seed(0); // make it deterministic

	for (size_t i = 0; i < 10; i++)
	{
		background.reset_direction(random);

		for (size_t i = 0; i < 100; i++)
		{
			background.update(FIXED_DELTA_60, (float)i);

			EXPECT_GE(background.get_offset().x, -(float)constants::background::TILE_SIZE);
			EXPECT_LT(background.get_offset().x, 0);
			EXPECT_GE(background.get_offset().y, -(float)constants::background::TILE_SIZE);
			EXPECT_LT(background.get_offset().y, 0);
		}
	}
}