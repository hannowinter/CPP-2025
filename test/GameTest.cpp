#include "TestCommon.hpp"

/*struct ControlTest : public testing::Test
{
protected:
	ControlTest()
	{
		GameControl& control = controls.request_add<GameControl>();
		controls.execute_requests();

		// remove any other controls that GameControl has added
		for (const auto& control : controls)
		{
			if (!control->is<GameControl>())
				controls.request_remove(&*control);
		}
		controls.execute_requests();

		// make randomness deterministic
		control.random().seed(0);
	}

	void add_proxy_alien_grid_control()
	{
		controls.request_add<AlienGridControl>();
		controls.execute_requests();

		// remove alien controls that AlienGridControl has added
		for (const auto& control : controls)
		{
			if (control->is<AlienControl>())
				controls.request_remove(&*control);
		}
		controls.execute_requests();
	}

	void add_proxy_player_control()
	{
		controls.request_add<PlayerControl>(sf::Vector2f{
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

	AlienBulletControl& alien_bullet = controls.request_add<AlienBulletControl>(sf::Vector2f{});
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
	controls.request_add<AlienControl>(Alien::RED, sf::Vector2f{}, 0, 0);
	controls.execute_requests();

	for (size_t i = 0; i < 2 * MAX_ITERATIONS_FOR_SHOOT; i++)
	{
		controls.update(FIXED_DELTA, mock_inputs);
	}

	controls.execute_requests();

	// at least 2 bullets should have been shot by now
	EXPECT_GE(controls.count<AlienBulletControl>(), 2);
}*/

int main2(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}