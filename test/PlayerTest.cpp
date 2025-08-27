#include "TestCommon.hpp"
#include "../src/control/PlayerControl.hpp"
#include "../src/model/GameState.hpp"
#include "../src/control/AlienBulletControl.hpp"
#include "../src/control/AlienControl.hpp"
#include "../src/control/UpgradeControl.hpp"

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

// Fixure for a control list containing a player control at { 100.0f, 200.0f }.
struct PlayerControlTest : testing::Test
{
protected:
	PlayerControlTest() :
		player{ controls.request_add<PlayerControl>(sf::Vector2f{ 100.0f, 200.0f }) }
	{
		controls.clear_init_requests();
		controls.execute_requests();

		state.lives = constants::game::INITIAL_LIVES;
	}

	ControlList controls;
	PlayerControl& player;
	GameState state;
};

// Tests whether the collisions with alien bullets are checked correctly.
TEST_F(PlayerControlTest, checkCollisionBulletTest)
{
	// should not collide
	AlienBulletControl& bullet1 = controls.request_add<AlienBulletControl>(sf::Vector2f{ 0.0f, 0.0f });
	controls.execute_requests();

	player.check_collision_bullet(bullet1, controls, state);
	controls.execute_requests();
	EXPECT_EQ(controls.count<AlienBulletControl>(), 1);
	EXPECT_EQ(state.lives, constants::game::INITIAL_LIVES);

	// should collide
	AlienBulletControl& bullet2 = controls.request_add<AlienBulletControl>(sf::Vector2f{ 100.0f, 200.0f });
	controls.execute_requests();

	player.check_collision_bullet(bullet2, controls, state);
	controls.execute_requests();
	EXPECT_EQ(controls.count<AlienBulletControl>(), 1); // remove bullet2 on collision
	EXPECT_TRUE(player.get_view().is_hit_animation());
	EXPECT_EQ(state.lives, constants::game::INITIAL_LIVES - 1);
}

// Tests whether the collisions with aliens are checked correctly.
TEST_F(PlayerControlTest, checkCollisionAlienTest)
{
	// should not collide
	AlienControl& alien1 = controls.request_add<AlienControl>(Alien::RED, sf::Vector2f{ 0.0f, 0.0f }, 0, 0);
	controls.clear_init_requests();
	controls.execute_requests();

	player.check_collision_alien(alien1, controls, state);
	controls.execute_requests();
	EXPECT_EQ(controls.count<AlienControl>(), 1);
	EXPECT_EQ(state.lives, constants::game::INITIAL_LIVES);

	// should collide, but is not in attack mode
	AlienControl& alien2 = controls.request_add<AlienControl>(Alien::RED, sf::Vector2f{ 100.0f, 200.0f }, 0, 0);
	controls.clear_init_requests();
	controls.execute_requests();

	player.check_collision_alien(alien2, controls, state);
	controls.execute_requests();
	EXPECT_EQ(controls.count<AlienControl>(), 2);
	EXPECT_FALSE(player.get_view().is_hit_animation());
	EXPECT_EQ(state.lives, constants::game::INITIAL_LIVES);

	// only now it collides
	alien2.start_swerve(sf::Vector2f{}, 0, 0);

	player.check_collision_alien(alien2, controls, state);
	controls.execute_requests();
	EXPECT_EQ(controls.count<AlienControl>(), 2); // don't remove alien
	EXPECT_TRUE(player.get_view().is_hit_animation());
	EXPECT_EQ(state.lives, constants::game::INITIAL_LIVES - 1);

	// should not collide a second time
	player.check_collision_alien(alien2, controls, state);
	controls.execute_requests();
	EXPECT_EQ(controls.count<AlienControl>(), 2); // don't remove alien
	EXPECT_EQ(state.lives, constants::game::INITIAL_LIVES - 1);
}

// Tests whether the collisions with upgrades are checked correctly.
TEST_F(PlayerControlTest, checkCollisionUpgradeTest)
{
	std::mt19937 random;
	random.seed(0);

	// should not collide, upgrade not spawned yet
	UpgradeControl& upgrade = controls.request_add<UpgradeControl>();
	controls.clear_init_requests();
	controls.execute_requests();

	player.check_collision_bullet(upgrade, controls, state);
	EXPECT_EQ(player.get_weapon(), constants::upgrades::Weapon::DEFAULT);
	EXPECT_FALSE(upgrade.is_picked_up());

	// spawned, but away from player
	upgrade.spawn_at(sf::Vector2f{ 0.0f, 0.0f }, constants::upgrades::Weapon::BOMB);

	player.check_collision_upgrade(upgrade, controls, random);
	EXPECT_EQ(player.get_weapon(), constants::upgrades::Weapon::DEFAULT);
	EXPECT_FALSE(upgrade.is_picked_up());

	// spawn inside player
	upgrade.spawn_at(sf::Vector2f{ 100.0f, 200.0f }, constants::upgrades::Weapon::BOMB);

	player.check_collision_upgrade(upgrade, controls, random);
	EXPECT_EQ(player.get_weapon(), constants::upgrades::Weapon::BOMB);
	EXPECT_TRUE(upgrade.is_picked_up());
}

TEST(PlayerViewTest, playerHitAnimationTest)
{
	PlayerView player;

	// not hit yet
	EXPECT_FALSE(player.is_hit_animation());
	EXPECT_EQ(player.get_animation_phase(), 0);

	// now hit
	player.play_hit_animation();
	EXPECT_TRUE(player.is_hit_animation());
	EXPECT_EQ(player.get_animation_phase(), 0);

	size_t phase_counter = 0;

	constexpr size_t MAX_ITERATIONS = 10000;
	for (size_t i = 0; ; i++)
	{
		if (i == MAX_ITERATIONS)
			FAIL();

		player.update(FIXED_DELTA_60);

		if (player.get_animation_phase() > phase_counter)
		{
			EXPECT_EQ(player.get_animation_phase(), phase_counter + 1);
			phase_counter++;
		}
		if (!player.is_hit_animation())
			break;
	}
}