#include "TestCommon.hpp"
#include "../src/control/AlienControl.hpp"

// fixture for Alien model related testing
// contains 3 Alien instances
struct AlienTest : public testing::Test
{
protected:
	constexpr static sf::Vector2f GRID_ORIGIN_1{ 0.0f, 0.0f };
	constexpr static sf::Vector2f GRID_ORIGIN_2{ 20.0f, 30.0f };
	constexpr static sf::Vector2f GRID_ORIGIN_3{ 500.0f, 400.0f };

	constexpr static size_t COLUMN_1 = 0;
	constexpr static size_t COLUMN_2 = 1;
	constexpr static size_t COLUMN_3 = 3;

	constexpr static size_t ROW_1 = 0;
	constexpr static size_t ROW_2 = 2;
	constexpr static size_t ROW_3 = 1;

	Alien alien1{ Alien::Variant::GREEN, GRID_ORIGIN_1, COLUMN_1, ROW_1 };
	Alien alien2{ Alien::Variant::RED, GRID_ORIGIN_2, COLUMN_2, ROW_2 };
	Alien alien3{ Alien::Variant::YELLOW, GRID_ORIGIN_3, COLUMN_3, ROW_3 };
};

// test whether "Alien::grid_offset" is implemented properly
// it should return the alien's offset to the grid's top-left corner
// the alien's size and spacing should be taken into account
TEST_F(AlienTest, alienGridOffset)
{
	constexpr sf::Vector2f SIZE = constants::alien::SIZE;
	constexpr sf::Vector2f SPACING = constants::alien_grid::SPACING;

	EXPECT_NEAR(alien1.grid_offset().x, 0.0f, EPS); // column 0
	EXPECT_NEAR(alien1.grid_offset().y, 0.0f, EPS); // row 0

	EXPECT_NEAR(alien2.grid_offset().x, SIZE.x + SPACING.x, EPS); // column 1
	EXPECT_NEAR(alien2.grid_offset().y, SIZE.y + SPACING.y + SIZE.y + SPACING.y, EPS); // row 2

	EXPECT_NEAR(alien3.grid_offset().x, SIZE.x + SPACING.x + SIZE.x + SPACING.x + SIZE.x + SPACING.x, EPS); // column 3
	EXPECT_NEAR(alien3.grid_offset().y, SIZE.y + SPACING.y, EPS); // row 1
}

// test whether the Alien constructor properly calculates its position based on the
// grid's origin point and the column and row number
TEST_F(AlienTest, alienPosition)
{
	constexpr sf::Vector2f SIZE = constants::alien::SIZE;
	constexpr sf::Vector2f SPACING = constants::alien_grid::SPACING;

	EXPECT_NEAR(alien1.hitbox().position.x, GRID_ORIGIN_1.x, EPS); // column 0
	EXPECT_NEAR(alien1.hitbox().position.y, GRID_ORIGIN_1.y, EPS); // row 0

	EXPECT_NEAR(alien2.hitbox().position.x, GRID_ORIGIN_2.x + SIZE.x + SPACING.x, EPS); // column 1
	EXPECT_NEAR(alien2.hitbox().position.y, GRID_ORIGIN_2.y + SIZE.y + SPACING.y + SIZE.y + SPACING.y, EPS); // row 2

	EXPECT_NEAR(alien3.hitbox().position.x, GRID_ORIGIN_3.x + SIZE.x + SPACING.x + SIZE.x + SPACING.x + SIZE.x + SPACING.x, EPS); // column 3
	EXPECT_NEAR(alien3.hitbox().position.y, GRID_ORIGIN_3.y + SIZE.y + SPACING.y, EPS); // row 1
}

// test fixure for testing ShakeState
struct ShakeTest : public testing::Test
{
protected:
	ShakeTest()
	{
		shake_state.timer = 0.0f;
		shake_state.start = { 0.0f, 0.0f };
		shake_state.target = { 0.0f, 0.0f };

		random.seed(0); // deterministic seeding
	}

	std::mt19937 random;
	ShakeState shake_state;
};

// test the ShakeState's "update" method in conjunction with the "finished_cycle" method
TEST_F(ShakeTest, updateFinishedTest)
{
	shake_state.duration = 1.0f;

	shake_state.update(0.5f);
	EXPECT_EQ(shake_state.timer, 0.5f);
	EXPECT_FALSE(shake_state.finished_cycle());

	shake_state.update(0.5f);
	EXPECT_EQ(shake_state.timer, 1.0f);
	EXPECT_TRUE(shake_state.finished_cycle());

	shake_state.update(0.25f);
	EXPECT_EQ(shake_state.timer, 1.0f); // should not overshoot
	EXPECT_TRUE(shake_state.finished_cycle()); // still true
}

// test the ShakeState's "new_cycle" method
TEST_F(ShakeTest, newCycleTest)
{
	shake_state.timer = 0.5f;
	shake_state.new_cycle(1.0f, random);

	// check if target is in desired range
	EXPECT_GE(shake_state.target.x, constants::alien::SHAKE_MIN_TARGET_OFFSET);
	EXPECT_LE(shake_state.target.x, constants::alien::SHAKE_MAX_TARGET_OFFSET);
	EXPECT_GE(shake_state.target.y, constants::alien::SHAKE_MIN_TARGET_OFFSET);
	EXPECT_LE(shake_state.target.y, constants::alien::SHAKE_MAX_TARGET_OFFSET);

	// check if duration is in desired range
	EXPECT_GE(shake_state.duration, constants::alien::SHAKE_MIN_DURATION);
	EXPECT_LE(shake_state.duration, constants::alien::SHAKE_MAX_DURATION);

	// check if timer was reset
	EXPECT_EQ(shake_state.timer, 0.0f);

	sf::Vector2f previous_target = shake_state.target;

	// start a new cycle
	shake_state.new_cycle(1.0f, random);

	// new start should be the previous target
	EXPECT_EQ(shake_state.start, previous_target);

	// start a new cycle but with higher intensity
	shake_state.new_cycle(4.0f, random);

	// the intensity should scale down the min and max duration
	EXPECT_GE(shake_state.duration, constants::alien::SHAKE_MIN_DURATION / 4.0f);
	EXPECT_LE(shake_state.duration, constants::alien::SHAKE_MAX_DURATION / 4.0f);
}

// test the ShakeState's offset calculation
TEST_F(ShakeTest, offsetTest)
{
	shake_state.start = { 1.0f, 1.0f };
	shake_state.target = { 5.0f, 9.0f };
	shake_state.timer = 0.0f;
	shake_state.duration = 1.0f;

	// the numbers have been chosen in such a way that floating point imprecisions shouldn't occur
	EXPECT_EQ(shake_state.get_offset(), shake_state.start);

	shake_state.update(0.25f);
	EXPECT_EQ(shake_state.get_offset(), (sf::Vector2f{ 2.0f, 3.0f })); // 1/4th of the way there

	shake_state.update(0.75f);
	EXPECT_EQ(shake_state.get_offset(), shake_state.target);
}