#include "TestCommon.hpp"
#include "../src/control/AlienControl.hpp"

// Spawns a new alien of specified column and row without initializing it.
AlienControl& spawn_mock_alien(ControlList& controls, size_t column, size_t row)
{
	AlienControl& alien = controls.request_add<AlienControl>(Alien::RED, sf::Vector2{ 0.0f, 0.0f }, column, row);
	controls.clear_init_requests(); // prevent errors from "GameControl" not being present
	controls.execute_requests();
	return alien;
}

// test whether the alien grid control has added all the aliens properly
TEST(AlienGridControlTest, addChildrenTest)
{
	ControlList controls;
	AlienGridControl alien_grid;
	alien_grid.add_children(controls);
	controls.clear_init_requests(); // prevent errors from "GameControl" not being present
	controls.execute_requests();

	constexpr size_t GREEN = constants::alien_grid::GREEN_ROWS;
	constexpr size_t YELLOW = constants::alien_grid::YELLOW_ROWS;
	constexpr size_t RED = constants::alien_grid::RED_ROWS;

	EXPECT_EQ(
		controls.count<AlienControl>(),
		constants::alien_grid::COLUMNS * (GREEN + YELLOW + RED)
	);

	size_t green_count = 0;
	size_t yellow_count = 0;
	size_t red_count = 0;
	for (const auto& control : controls)
	{
		if (AlienControl* alien = control->is<AlienControl>())
		{
			switch (alien->get().variant())
			{
			case Alien::GREEN:
				green_count++;
				break;
			case Alien::YELLOW:
				yellow_count++;
				break;
			case Alien::RED:
				red_count++;
				break;
			}
		}
	}
	EXPECT_EQ(green_count, constants::alien_grid::COLUMNS * GREEN);
	EXPECT_EQ(yellow_count, constants::alien_grid::COLUMNS * YELLOW);
	EXPECT_EQ(red_count, constants::alien_grid::COLUMNS * RED);
}

// test the "get_min_max_column" and "get_min_max_row" functions
// we won't need to test the case where no aliens exist as the game immediately stops in that case
TEST(AlienGridControlTest, getMinMaxColumnRowTest)
{
	constexpr sf::Vector2f ORIGIN{ 0.0f, 0.0f };

	constexpr size_t ALIEN1_COLUMN = 3;
	constexpr size_t ALIEN1_ROW = 4;

	constexpr size_t ALIEN2_COLUMN = 7;
	constexpr size_t ALIEN2_ROW = 4;

	constexpr size_t ALIEN3_COLUMN = 5;
	constexpr size_t ALIEN3_ROW = 6;

	ControlList controls;

	// ================================================================================================

	spawn_mock_alien(controls, ALIEN1_COLUMN, ALIEN1_ROW);

	// with only 1 alien, min and max should all be equal

	auto [min_row1, max_row1] = AlienGridControl::get_min_max_row(controls);
	auto [min_col1, max_col1] = AlienGridControl::get_min_max_column(controls);

	EXPECT_EQ(min_row1, ALIEN1_ROW);
	EXPECT_EQ(max_row1, ALIEN1_ROW);
	EXPECT_EQ(min_col1, ALIEN1_COLUMN);
	EXPECT_EQ(max_col1, ALIEN1_COLUMN);

	// ================================================================================================

	spawn_mock_alien(controls, ALIEN2_COLUMN, ALIEN2_ROW);

	// alien1 and alien2 are on the same row
	// alien1 is the leftmost alien
	// alien2 is the rightmost alien

	auto [min_row2, max_row2] = AlienGridControl::get_min_max_row(controls);
	auto [min_col2, max_col2] = AlienGridControl::get_min_max_column(controls);

	EXPECT_EQ(min_row2, ALIEN1_ROW);
	EXPECT_EQ(max_row2, ALIEN1_ROW);
	EXPECT_EQ(min_col2, ALIEN1_COLUMN);
	EXPECT_EQ(max_col2, ALIEN2_COLUMN);

	// ================================================================================================

	spawn_mock_alien(controls, ALIEN3_COLUMN, ALIEN3_ROW);

	// alien1 and alien2 are on the same row and both the topmost aliens
	// alien 3 is the bottommost alien
	// alien1 is the leftmost alien
	// alien2 is the rightmost alien

	auto [min_row3, max_row3] = AlienGridControl::get_min_max_row(controls);
	auto [min_col3, max_col3] = AlienGridControl::get_min_max_column(controls);

	EXPECT_EQ(min_row3, ALIEN1_ROW);
	EXPECT_EQ(max_row3, ALIEN3_ROW);
	EXPECT_EQ(min_col3, ALIEN1_COLUMN);
	EXPECT_EQ(max_col3, ALIEN2_COLUMN);
}

TEST(AlienGridControlTest, startRandomSwerveTest)
{
	ControlList controls;
	std::mt19937 random;
	random.seed(0);

	constexpr size_t ALIEN1_COLUMN = 1;
	constexpr size_t ALIEN1_ROW = 2;

	constexpr size_t ALIEN2_COLUMN = 3;
	constexpr size_t ALIEN2_ROW = 4;

	constexpr size_t ALIEN3_COLUMN = 5;
	constexpr size_t ALIEN3_ROW = 6;

	AlienControl& alien1 = spawn_mock_alien(controls, ALIEN1_COLUMN, ALIEN1_ROW);

	// only alien1 can be picked now
	AlienGridControl::start_random_swerve(random, controls, { 0.0f, 0.0f });

	EXPECT_EQ(alien1.get_mode(), AlienControl::SWERVE);
	EXPECT_EQ(alien1.get_swerve_state().target_column, ALIEN1_COLUMN);
	EXPECT_EQ(alien1.get_swerve_state().target_row, ALIEN1_ROW);

	// check repeatedly if the swerving aliens are always distinct
	for (size_t i = 0; i < 20; i++)
	{
		// clear all
		for (const auto& control : controls)
			controls.request_remove(&*control);
		controls.execute_requests();

		AlienControl& alien1 = spawn_mock_alien(controls, ALIEN1_COLUMN, ALIEN1_ROW);
		AlienControl& alien2 = spawn_mock_alien(controls, ALIEN2_COLUMN, ALIEN2_ROW);
		AlienControl& alien3 = spawn_mock_alien(controls, ALIEN3_COLUMN, ALIEN3_ROW);

		AlienGridControl::start_random_swerve(random, controls, { 0.0f, 0.0f });

		std::vector<AlienControl*> swerving_aliens;
		if (alien1.get_mode() == AlienControl::SWERVE)
			swerving_aliens.push_back(&alien1);
		if (alien2.get_mode() == AlienControl::SWERVE)
			swerving_aliens.push_back(&alien2);
		if (alien3.get_mode() == AlienControl::SWERVE)
			swerving_aliens.push_back(&alien3);

		// exactly 2 aliens must be swerving
		EXPECT_EQ(swerving_aliens.size(), 2);

		// check if they swap columns and rows
		EXPECT_EQ(swerving_aliens[0]->get().column(), swerving_aliens[1]->get_swerve_state().target_column);
		EXPECT_EQ(swerving_aliens[0]->get().row(), swerving_aliens[1]->get_swerve_state().target_row);
		EXPECT_EQ(swerving_aliens[1]->get().column(), swerving_aliens[0]->get_swerve_state().target_column);
		EXPECT_EQ(swerving_aliens[1]->get().row(), swerving_aliens[0]->get_swerve_state().target_row);
	}
}