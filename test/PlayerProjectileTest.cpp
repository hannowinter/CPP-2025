#include "TestCommon.hpp"
#include "../src/control/PlayerProjectileControl.hpp"
#include "../src/model/PlayerBullet.hpp"
#include "../src/model/PlayerLaser.hpp"
#include "../src/model/PlayerBomb.hpp"

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