#include <gtest/gtest.h>
// #include <gmock/gmock.h>
#include "../src/control/Controls.hpp"
#include "../src/control/GameControl.hpp"
#include "../src/control/AlienBulletControl.hpp"
#include "../src/control/AlienControl.hpp"
#include "../src/control/AlienGridControl.hpp"
#include "../src/control/PlayerControl.hpp"
#include "../src/model/Constants.hpp"

constexpr float FIXED_DELTA = 1.0f / 60;

// proxy control class used for testing
// if "AddChildren" is true, then it will add a child of index I + 10, 
// as long as this new index is less than or equal to 50
template <size_t I, bool AddChildren = false>
struct ProxyControl : public Control
{
	size_t data;
	bool initialized = false;
	size_t updates = 0;
	float delta_sum = 0.0f;
	size_t draws = 0;

	ProxyControl(size_t data) :
		data{ data } { }

	void add_children(ControlList& controls) override
	{
		if constexpr (AddChildren && I + 10 <= 50)
		{
			controls.add<ProxyControl<I + 10, true>>(data);
		}
	}

	void init(const ControlList& controls) override
	{
		initialized = true;
	}

	void update(const UpdateState& state) override
	{
		updates++;
		delta_sum += state.delta;
	}

	void draw(LayerManager& layers) override
	{
		draws++;
	}
};

struct ControlListTest : public testing::Test
{
protected:
	ControlListTest() :
		proxy_window{ sf::VideoMode{ { constants::VIEW_WIDTH, constants::VIEW_HEIGHT } }, "test" },
		proxy_layers{ proxy_window }
	{

	}

	ControlList controls;
	const Inputs inputs_proxy;
	sf::RenderWindow proxy_window;
	LayerManager proxy_layers;
};

TEST_F(ControlListTest, controlListAddRemove)
{
	// contains no controls initially
	EXPECT_EQ(std::ranges::size(controls), 0);

	ProxyControl<0>& control41 = controls.add<ProxyControl<0>>(41);
	ProxyControl<0>& control17 = controls.add<ProxyControl<0>>(17);
	ProxyControl<1>& control67 = controls.add<ProxyControl<1>>(67);

	// still no elements because we haven't called "execute_requests" yet
	EXPECT_EQ(std::ranges::size(controls), 0);
	EXPECT_EQ(controls.count<ProxyControl<0>>(), 0);
	EXPECT_EQ(controls.count<ProxyControl<1>>(), 0);

	controls.execute_requests();

	// check the expected counts
	EXPECT_EQ(std::ranges::size(controls), 3);
	EXPECT_EQ(controls.count<ProxyControl<0>>(), 2);
	EXPECT_EQ(controls.count<ProxyControl<1>>(), 1);
	EXPECT_EQ(controls.count<ProxyControl<2>>(), 0);

	// check if the controls are accessible via "get" and in the correct order
	EXPECT_EQ(controls.get<ProxyControl<0>>(0)->data, control41.data);
	EXPECT_EQ(controls.get<ProxyControl<0>>(1)->data, control17.data);
	EXPECT_EQ(controls.get<ProxyControl<1>>(0)->data, control67.data);

	// check if "get" returns nullptr for controls that are not contained
	EXPECT_EQ(controls.get<ProxyControl<0>>(3), nullptr);
	EXPECT_EQ(controls.get<ProxyControl<2>>(0), nullptr);

	controls.remove(&control41);
	controls.remove(&control67);

	// no change because we haven't calledd "execute_requests" yet
	EXPECT_EQ(std::ranges::size(controls), 3);

	controls.execute_requests();

	// check the expected new counts
	EXPECT_EQ(std::ranges::size(controls), 1);
	EXPECT_EQ(controls.count<ProxyControl<0>>(), 1);
	EXPECT_EQ(controls.count<ProxyControl<1>>(), 0);

	// control17 should now be at index 0 instead of index 1
	EXPECT_EQ(controls.get<ProxyControl<0>>(0)->data, control17.data);

	// check if trying to get the controls we removed now yields nullptr
	EXPECT_EQ(controls.get<ProxyControl<0>>(1), nullptr);
	EXPECT_EQ(controls.get<ProxyControl<1>>(0), nullptr);
}

TEST_F(ControlListTest, controlListInitUpdateDraw)
{
	ProxyControl<0>& control0 = controls.add<ProxyControl<0>>(0);
	ProxyControl<1>& control1 = controls.add<ProxyControl<1>>(0);

	// should not be initialized yet
	EXPECT_FALSE(control0.initialized);
	EXPECT_FALSE(control1.initialized);

	controls.execute_requests();

	// only gets initialized when the "add" request gets executed
	EXPECT_TRUE(control0.initialized);
	EXPECT_TRUE(control1.initialized);

	// no updates or draws yet
	EXPECT_EQ(control0.updates, 0);
	EXPECT_EQ(control1.updates, 0);
	EXPECT_EQ(control0.draws, 0);
	EXPECT_EQ(control1.draws, 0);

	controls.update(1.0f, inputs_proxy);
	controls.update(2.0f, inputs_proxy);
	controls.update(3.0f, inputs_proxy);

	controls.draw(proxy_layers);
	controls.draw(proxy_layers);

	// check the expected count of updates and draws
	EXPECT_EQ(control0.updates, 3);
	EXPECT_EQ(control0.delta_sum, 1.0f + 2.0f + 3.0f); // (floating point imprecisions won't occur with small whole numbers)
	EXPECT_EQ(control0.draws, 2);
}

TEST_F(ControlListTest, controlListAddChildren)
{
	ProxyControl<0, true>& control0 = controls.add<ProxyControl<0, true>>(0);
	ProxyControl<15, true>& control15 = controls.add<ProxyControl<15, true>>(0);
	ProxyControl<40, true>& control40 = controls.add<ProxyControl<40, true>>(0);

	controls.execute_requests();

	// check the expected counts
	EXPECT_EQ((controls.count<ProxyControl<0, true>>()), 1);
	EXPECT_EQ((controls.count<ProxyControl<10, true>>()), 1);
	EXPECT_EQ((controls.count<ProxyControl<20, true>>()), 1);
	EXPECT_EQ((controls.count<ProxyControl<30, true>>()), 1);
	EXPECT_EQ((controls.count<ProxyControl<40, true>>()), 2);
	EXPECT_EQ((controls.count<ProxyControl<50, true>>()), 2);
	EXPECT_EQ((controls.count<ProxyControl<15, true>>()), 1);
	EXPECT_EQ((controls.count<ProxyControl<25, true>>()), 1);
	EXPECT_EQ((controls.count<ProxyControl<35, true>>()), 1);
	EXPECT_EQ((controls.count<ProxyControl<45, true>>()), 1);

	// should not go higher than 50
	EXPECT_EQ((controls.count<ProxyControl<60, true>>()), 0);
	EXPECT_EQ((controls.count<ProxyControl<55, true>>()), 0);
}

struct ControlTest : public testing::Test
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
	const Inputs inputs_proxy;
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
		controls.update(FIXED_DELTA, inputs_proxy);
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
		controls.update(FIXED_DELTA, inputs_proxy);
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
		controls.update(FIXED_DELTA, inputs_proxy);

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
		controls.update(FIXED_DELTA, inputs_proxy);

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
		controls.update(FIXED_DELTA, inputs_proxy);

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
}

int main2(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}