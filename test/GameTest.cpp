#include "TestCommon.hpp"

/*struct ControlTest : public testing::Test
{
protected:
	ControlTest()
	{
		GameControl& control = controls.add<GameControl>();
		controls.execute_requests();

		// remove any other controls that GameControl has added
		for (const auto& control : controls)
		{
			if (!control->is<GameControl>())
				controls.remove(&*control);
		}
		controls.execute_requests();

		// make randomness deterministic
		control.random().seed(0);
	}

	void add_proxy_alien_grid_control()
	{
		controls.add<AlienGridControl>();
		controls.execute_requests();

		// remove alien controls that AlienGridControl has added
		for (const auto& control : controls)
		{
			if (control->is<AlienControl>())
				controls.remove(&*control);
		}
		controls.execute_requests();
	}

	void add_proxy_player_control()
	{
		controls.add<PlayerControl>(sf::Vector2f{
			(constants::VIEW_WIDTH - constants::player::SIZE.x) / 2.0f,
			constants::VIEW_HEIGHT - constants::player::SIZE.y - constants::PADDING
		});
		controls.execute_requests();
	}

	ControlList controls;
	const Inputs mock_inputs;
};

TEST_F(ControlTest, alienBulletOutsideView)
{
	constexpr float TIME_FOR_OUTSIDE_VIEW = constants::VIEW_HEIGHT / constants::alien_bullet::MOVE_SPEED;
	constexpr size_t ITERATIONS_FOR_OUTSIDE_VIEW = 
		TIME_FOR_OUTSIDE_VIEW / FIXED_DELTA +
		1; // add a bit of leniency

	AlienBulletControl& alien_bullet = controls.add<AlienBulletControl>(sf::Vector2f{});
	controls.execute_requests();

	for (size_t i = 0; i < ITERATIONS_FOR_OUTSIDE_VIEW; i++)
	{
		controls.update(FIXED_DELTA, mock_inputs);
	}

	// check if bullet is below view
	EXPECT_GT(alien_bullet.get().hitbox().position.y, constants::VIEW_HEIGHT);

	// check if bullet got removed
	controls.execute_requests();
	EXPECT_EQ(controls.count<AlienBulletControl>(), 0);
}

TEST_F(ControlTest, alienShooting)
{
	constexpr float MAX_ITERATIONS_FOR_SHOOT = 
		constants::alien::MAX_SHOOT_TIME / FIXED_DELTA +
		1; // add a bit of leniency
	
	add_proxy_alien_grid_control(); // necessary to prevent crashing
	add_proxy_player_control(); // necessary to prevent crashing
	controls.add<AlienControl>(Alien::RED, sf::Vector2f{}, 0, 0);
	controls.execute_requests();

	for (size_t i = 0; i < 2 * MAX_ITERATIONS_FOR_SHOOT; i++)
	{
		controls.update(FIXED_DELTA, mock_inputs);
	}

	controls.execute_requests();

	// at least 2 bullets should have been shot by now
	EXPECT_GE(controls.count<AlienBulletControl>(), 2);
}

TEST_F(ControlTest, alienSwervingAndRetreating)
{
	constexpr size_t ALIEN1_INIT_COLUMN = 0;
	constexpr size_t ALIEN2_INIT_COLUMN = 1;

	constexpr float MAX_ITERATIONS_FOR_SWERVE =
		constants::alien_grid::SWERVE_MAX_WAIT / FIXED_DELTA +
		1; // add a bit of leniency

	add_proxy_alien_grid_control(); // necessary to prevent crashing
	add_proxy_player_control(); // necessary to prevent crashing
	AlienControl& alien1 = controls.add<AlienControl>(Alien::RED, sf::Vector2f{}, ALIEN1_INIT_COLUMN, 0);
	AlienControl& alien2 = controls.add<AlienControl>(Alien::GREEN, sf::Vector2f{}, ALIEN2_INIT_COLUMN, 0);
	controls.execute_requests();

	// check if aliens initiate a swerve after some time
	for (size_t i = 0; i < MAX_ITERATIONS_FOR_SWERVE; i++)
	{
		controls.update(FIXED_DELTA, mock_inputs);

		if (alien1.get_mode() == AlienControl::SWERVE || alien2.get_mode() == AlienControl::SWERVE)
		{
			// if one is swerving, then so should the other
			EXPECT_EQ(alien1.get_mode(), AlienControl::SWERVE);
			EXPECT_EQ(alien2.get_mode(), AlienControl::SWERVE);

			goto test_retreat_initiation;
		}
	}

	// swerve never occured
	FAIL();

test_retreat_initiation:
	constexpr float MAX_TIME_FOR_RETREAT = 10.0f; // a swerve should not take this long
	constexpr size_t MAX_ITERATIONS_FOR_RETREAT =
		MAX_TIME_FOR_RETREAT / FIXED_DELTA;

	bool alien1_retreating = false;
	bool alien2_retreating = false;

	// check if aliens retreat after some time
	for (size_t i = 0; i < MAX_ITERATIONS_FOR_RETREAT; i++)
	{
		controls.update(FIXED_DELTA, mock_inputs);

		if (!alien1_retreating && alien1.get_mode() != AlienControl::SWERVE)
		{
			// if it's not swerving anymore, it should be retreating
			EXPECT_EQ(alien1.get_mode(), AlienControl::RETREAT);
			// check if alien is below threshold
			EXPECT_GE(alien1.get().hitbox().position.y, constants::VIEW_HEIGHT - constants::alien::RETREAT_THRESHOLD);
			
			alien1_retreating = true;
		}
		if (!alien2_retreating && alien2.get_mode() != AlienControl::SWERVE)
		{
			// if it's not swerving anymore, it should be retreating
			EXPECT_EQ(alien2.get_mode(), AlienControl::RETREAT);
			// check if alien is below threshold
			EXPECT_GE(alien2.get().hitbox().position.y, constants::VIEW_HEIGHT - constants::alien::RETREAT_THRESHOLD);
			
			alien2_retreating = true;
		}

		if (alien1_retreating && alien2_retreating)
			goto test_retreat_finish;
	}

	FAIL();

test_retreat_finish:
	constexpr float MAX_TIME_FOR_RETREAT_FINISH = 10.0f; // a retreat should not take this long
	constexpr size_t MAX_ITERATIONS_FOR_RETREAT_FINISH =
		MAX_TIME_FOR_RETREAT_FINISH / FIXED_DELTA;

	bool alien1_finished = false;
	bool alien2_finished = false;

	// check if aliens finish retreating after some time
	for (size_t i = 0; i < MAX_ITERATIONS_FOR_RETREAT_FINISH; i++)
	{
		controls.update(FIXED_DELTA, mock_inputs);

		if (!alien1_finished && alien1.get_mode() != AlienControl::RETREAT)
		{
			// if it's not retreating anymore, it should be grid aligned
			EXPECT_EQ(alien1.get_mode(), AlienControl::GRID_ALIGNED);

			alien1_finished = true;
		}
		if (!alien2_finished && alien2.get_mode() != AlienControl::RETREAT)
		{
			// if it's not retreating anymore, it should be grid aligned
			EXPECT_EQ(alien2.get_mode(), AlienControl::GRID_ALIGNED);

			alien2_finished = true;
		}

		if (alien1_finished && alien2_finished)
			goto test_aliens_swapped;
	}

	FAIL();

test_aliens_swapped:
	EXPECT_EQ(alien1.get().column(), ALIEN2_INIT_COLUMN);
	EXPECT_EQ(alien2.get().column(), ALIEN1_INIT_COLUMN);
}*/

int main2(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}