#include <gtest/gtest.h>
// #include <gmock/gmock.h>
#include "../src/control/Controls.hpp"
#include "../src/control/GameControl.hpp"
#include "../src/control/AlienBulletControl.hpp"
#include "../src/control/AlienControl.hpp"
#include "../src/control/AlienGridControl.hpp"
#include "../src/control/PlayerControl.hpp"
#include "../src/model/PlayerBullet.hpp"
#include "../src/model/PlayerLaser.hpp"
#include "../src/model/PlayerBomb.hpp"
#include "../src/model/Constants.hpp"

constexpr float EPS = 0.001f;

constexpr float FIXED_DELTA_30 = 1.0f / 30; // 30 fps
constexpr float FIXED_DELTA_60 = 1.0f / 60; // 60 fps
constexpr float FIXED_DELTA_120 = 1.0f / 120; // 120 fps

// moch control class used for testing
// if "AddChildren" is true, then it will add a child of index I + 10, 
// as long as this new index is less than or equal to 50
template <size_t I, bool AddChildren = false>
struct MockControl : public Control
{
	size_t data;
	bool initialized = false;
	size_t updates = 0;
	float delta_sum = 0.0f;
	size_t draws = 0;

	MockControl(size_t data) :
		data{ data } { }

	void add_children(ControlList& controls) override
	{
		if constexpr (AddChildren && I + 10 <= 50)
		{
			controls.add<MockControl<I + 10, true>>(data);
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

// fixture for testing the ControlList class, containing all the
// necessary mock objects for calling the update and draw cycles
struct ControlListTest : public testing::Test
{
protected:
	ControlListTest() :
		mock_window{ sf::VideoMode{ { constants::VIEW_WIDTH, constants::VIEW_HEIGHT } }, "test" },
		mock_layers{ mock_window }
	{

	}

	ControlList controls;
	const Inputs mock_inputs;
	sf::RenderWindow mock_window;
	LayerManager mock_layers;
};

// testing whether querying list information using "count" and "get" gives the expected results
// in conjunction with list modifications by "add" and "remove"
TEST_F(ControlListTest, controlListAddRemoveCountGet)
{
	// contains no controls initially
	EXPECT_EQ(std::ranges::size(controls), 0);

	MockControl<0>& control41 = controls.add<MockControl<0>>(41);
	MockControl<0>& control17 = controls.add<MockControl<0>>(17);
	MockControl<1>& control67 = controls.add<MockControl<1>>(67);

	// still no elements because we haven't called "execute_requests" yet
	EXPECT_EQ(std::ranges::size(controls), 0);
	EXPECT_EQ(controls.count<MockControl<0>>(), 0);
	EXPECT_EQ(controls.count<MockControl<1>>(), 0);

	controls.execute_requests();

	// check the expected counts
	EXPECT_EQ(std::ranges::size(controls), 3);
	EXPECT_EQ(controls.count<MockControl<0>>(), 2);
	EXPECT_EQ(controls.count<MockControl<1>>(), 1);
	EXPECT_EQ(controls.count<MockControl<2>>(), 0);

	// check if the controls are accessible via "get" and in the correct order
	EXPECT_EQ(controls.get<MockControl<0>>(0)->data, control41.data);
	EXPECT_EQ(controls.get<MockControl<0>>(1)->data, control17.data);
	EXPECT_EQ(controls.get<MockControl<1>>(0)->data, control67.data);

	// check if "get" returns nullptr for controls that are not contained
	EXPECT_EQ(controls.get<MockControl<0>>(3), nullptr);
	EXPECT_EQ(controls.get<MockControl<2>>(0), nullptr);

	controls.remove(&control41);
	controls.remove(&control67);

	// no change because we haven't called "execute_requests" yet
	EXPECT_EQ(std::ranges::size(controls), 3);

	controls.execute_requests();

	// check the expected new counts
	EXPECT_EQ(std::ranges::size(controls), 1);
	EXPECT_EQ(controls.count<MockControl<0>>(), 1);
	EXPECT_EQ(controls.count<MockControl<1>>(), 0);

	// control17 should now be at index 0 instead of index 1
	EXPECT_EQ(controls.get<MockControl<0>>(0)->data, control17.data);

	// check if trying to get the controls we removed now yields nullptr
	EXPECT_EQ(controls.get<MockControl<0>>(1), nullptr);
	EXPECT_EQ(controls.get<MockControl<1>>(0), nullptr);

	// remove the last one and check if it's now empty
	controls.remove(controls.get<MockControl<0>>(0));
	controls.execute_requests();
	EXPECT_EQ(std::ranges::size(controls), 0);
}

// testing whether the "ControlList::init", "ControlList::update" and "ControlList::draw" functions 
// properly call the respective "Control::init", "Control::update" and "Control::draw" function on all
// of its elements
TEST_F(ControlListTest, controlListInitUpdateDraw)
{
	MockControl<0>& control0 = controls.add<MockControl<0>>(0);
	MockControl<1>& control1 = controls.add<MockControl<1>>(0);

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

	controls.update(1.0f, mock_inputs);
	controls.update(2.0f, mock_inputs);
	controls.update(3.0f, mock_inputs);

	controls.draw(mock_layers);
	controls.draw(mock_layers);
	controls.draw(mock_layers);
	controls.draw(mock_layers);

	// check the expected count of updates and draws
	EXPECT_EQ(control0.updates, 3);
	EXPECT_EQ(control0.delta_sum, 1.0f + 2.0f + 3.0f); // (floating point imprecisions won't occur with small whole numbers)
	EXPECT_EQ(control0.draws, 4);
}

// testing whether adding controls that each also add child controls behaves properly
TEST_F(ControlListTest, controlListAddChildren)
{
	// By passing "true" to the template, each MockControl will now recursively spawn a child of index I+10,
	// as long as I+10 <= 50.
	MockControl<0, true>& control0 = controls.add<MockControl<0, true>>(0);
	MockControl<15, true>& control15 = controls.add<MockControl<15, true>>(0);
	MockControl<40, true>& control40 = controls.add<MockControl<40, true>>(0);

	controls.execute_requests();

	// check the expected counts
	// We started with indices 0, 15 and 40, so we should expect the following children:
	// 0 -> 10  ->  20  ->  30  ->  40  ->  50
	//          15  ->  25  ->  35  ->  45
	//                              40  ->  50  
	EXPECT_EQ((controls.count<MockControl<0, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<10, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<20, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<30, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<40, true>>()), 2);
	EXPECT_EQ((controls.count<MockControl<50, true>>()), 2);
	EXPECT_EQ((controls.count<MockControl<15, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<25, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<35, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<45, true>>()), 1);

	// should not go higher than 50
	EXPECT_EQ((controls.count<MockControl<60, true>>()), 0);
	EXPECT_EQ((controls.count<MockControl<55, true>>()), 0);
}

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

// fixture for AlienBullet model related testing
// contains 3 AlienBullet instances
struct AlienBulletTest : public testing::Test
{
protected:
	constexpr static sf::Vector2f POSITION_1{ 0.0f, 200.0f };
	constexpr static sf::Vector2f POSITION_2{ 20.0f, 200.0f };
	constexpr static sf::Vector2f POSITION_3{ 500.0f, 200.0f };

	AlienBullet alien_bullet1{ POSITION_1 };
	AlienBullet alien_bullet2{ POSITION_2 };
	AlienBullet alien_bullet3{ POSITION_3 };
};

// test whether "AlienBullet::move_down" behaves properly, also in conjunction with different framerates
TEST_F(AlienBulletTest, alienBulletMoveDown)
{
	constexpr float MOVE_SPEED = constants::alien_bullet::MOVE_SPEED;

	// moves once, 30fps
	alien_bullet1.move_down(FIXED_DELTA_30);

	// moves twice, 60fps
	alien_bullet2.move_down(FIXED_DELTA_60);
	alien_bullet2.move_down(FIXED_DELTA_60);
	
	// moves four times, 120fps
	for (size_t i = 0; i < 4; i++)
		alien_bullet3.move_down(FIXED_DELTA_120);

	// by letting all bullets start at y=200 and using 30fps, 60fps and 120fps movements, 
	// these should all be equal
	EXPECT_NEAR(alien_bullet1.hitbox().position.y, alien_bullet2.hitbox().position.y, EPS);
	EXPECT_NEAR(alien_bullet2.hitbox().position.y, alien_bullet3.hitbox().position.y, EPS);
	EXPECT_NEAR(alien_bullet3.hitbox().position.y, alien_bullet1.hitbox().position.y, EPS);

	// confirm movement has occurred
	EXPECT_NEAR(alien_bullet1.hitbox().position.y, 200.0f + MOVE_SPEED * FIXED_DELTA_30, EPS);

	// x-position should be unchanged
	EXPECT_EQ(alien_bullet1.hitbox().position.x, POSITION_1.x);
	EXPECT_EQ(alien_bullet2.hitbox().position.x, POSITION_2.x);
	EXPECT_EQ(alien_bullet3.hitbox().position.x, POSITION_3.x);
}

// fixture for Player model related testing
// contains 3 Player instances
struct PlayerTest : public testing::Test
{
protected:
	constexpr static sf::Vector2f POSITION_1{ 0.0f, 400.0f };
	constexpr static sf::Vector2f POSITION_2{ 200.0f, 200.0f };
	constexpr static sf::Vector2f POSITION_3{ 5000.0f, 300.0f };

	Player player1{ POSITION_1 };
	Player player2{ POSITION_2 };
	Player player3{ POSITION_3 };
};

// test whether "Player::move" behaves properly and moves to the correct direction
TEST_F(PlayerTest, playerMove)
{
	player1.move(Player::LEFT, FIXED_DELTA_60);
	player2.move(Player::RIGHT, FIXED_DELTA_60);
	player3.move(Player::LEFT, FIXED_DELTA_60);

	EXPECT_LT(player1.hitbox().position.x, POSITION_1.x); // left
	EXPECT_GT(player2.hitbox().position.x, POSITION_2.x); // right
	EXPECT_LT(player3.hitbox().position.x, POSITION_3.x); // left

	// y-position should be unchanged
	EXPECT_EQ(player1.hitbox().position.y, POSITION_1.y);
	EXPECT_EQ(player2.hitbox().position.y, POSITION_2.y);
	EXPECT_EQ(player3.hitbox().position.y, POSITION_3.y);

	// move back
	player1.move(Player::RIGHT, FIXED_DELTA_60);
	player2.move(Player::LEFT, FIXED_DELTA_60);
	player3.move(Player::RIGHT, FIXED_DELTA_60);

	EXPECT_NEAR(player1.hitbox().position.x, POSITION_1.x, EPS);
	EXPECT_NEAR(player2.hitbox().position.x, POSITION_2.x, EPS);
	EXPECT_NEAR(player3.hitbox().position.x, POSITION_3.x, EPS);
}

// test whether "Player::clamp_position" correctly snaps the player "constants::PADDING" away from the view's border
TEST_F(PlayerTest, playerClamp)
{
	// player 1 is too far left with x=0
	// player 2 is positioned correctly with x=200
	// player 3 is too far right with x=5000

	player1.clamp_position();
	player2.clamp_position();
	player3.clamp_position();

	EXPECT_GT(player1.hitbox().position.x, POSITION_1.x);
	EXPECT_EQ(player2.hitbox().position.x, POSITION_2.x);
	EXPECT_LT(player3.hitbox().position.x, POSITION_3.x);

	// precisely calculate the expected position
	EXPECT_NEAR(player1.hitbox().position.x, constants::PADDING, EPS);
	EXPECT_NEAR(player3.hitbox().position.x, constants::VIEW_WIDTH - constants::PADDING - constants::player::SIZE.x, EPS);

	// y-position should be unchanged
	EXPECT_EQ(player1.hitbox().position.y, POSITION_1.y);
	EXPECT_EQ(player2.hitbox().position.y, POSITION_2.y);
	EXPECT_EQ(player3.hitbox().position.y, POSITION_3.y);
}

// fixture for PlayerProjectile model and its child model
struct PlayerProjectiveTest : public testing::Test
{
	constexpr static sf::Vector2f POSITION_BULLET{ 0.0f, 200.0f };
	constexpr static sf::Vector2f POSITION_LASER{ 20.0f, 200.0f };
	constexpr static sf::Vector2f POSITION_BOMB{ 500.0f, 200.0f };

	PlayerBullet player_bullet{ POSITION_BULLET };
	PlayerLaser player_laser{ POSITION_LASER };
	PlayerBomb player_bomb{ POSITION_BOMB };
};

// test whether "PlayerProjectile::move_up" behaves properly, also in conjunction with different framerates
TEST_F(PlayerProjectiveTest, playerProjectileMoveUp)
{
	constexpr float MOVE_SPEED = constants::player_bullet::MOVE_SPEED;

	// moves once, 30fps
	player_bullet.move_up(FIXED_DELTA_30);

	// moves twice, 60fps
	player_laser.move_up(FIXED_DELTA_60);
	player_laser.move_up(FIXED_DELTA_60);

	// moves four times, 120fps
	for (size_t i = 0; i < 4; i++)
		player_bomb.move_up(FIXED_DELTA_120);

	// by letting all projectiles start at y=200 and using 30fps, 60fps and 120fps movements, 
	// these should all be equal
	EXPECT_NEAR(player_bullet.hitbox().position.y, player_laser.hitbox().position.y, EPS);
	EXPECT_NEAR(player_laser.hitbox().position.y, player_bomb.hitbox().position.y, EPS);
	EXPECT_NEAR(player_bomb.hitbox().position.y, player_bullet.hitbox().position.y, EPS);

	// confirm movement has occurred
	EXPECT_NEAR(player_bullet.hitbox().position.y, 200.0f - MOVE_SPEED * FIXED_DELTA_30, EPS);

	// x-position should be unchanged
	EXPECT_EQ(player_bullet.hitbox().position.x, POSITION_BULLET.x);
	EXPECT_EQ(player_laser.hitbox().position.x, POSITION_LASER.x);
	EXPECT_EQ(player_bomb.hitbox().position.x, POSITION_BOMB.x);
}

// test whether "PlayerBomb" gives the correct hitbox before and after exploding
TEST_F(PlayerProjectiveTest, bombExplodeHitbox)
{
	// not yet exploded
	EXPECT_EQ(player_bomb.hitbox().size, constants::player_bullet::BOMB_SIZE);

	player_bomb.explode();

	// exploded
	EXPECT_EQ(player_bomb.hitbox().size, constants::player_bullet::EXPLOSION_SIZE);
}

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