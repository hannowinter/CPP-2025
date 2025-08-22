#include "TestCommon.hpp"
#include "../src/control/PlayerControl.hpp"

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