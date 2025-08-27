#include "TestCommon.hpp"
#include "../src/control/PlayerProjectileControl.hpp"

// Tests whether the bomb exploding behaves correctly.
TEST(BombControlTest, explodeTest)
{
	ControlList controls;
	BombControl& bomb = controls.request_add<BombControl>(sf::Vector2f{});
	controls.execute_requests();

	// no explosion yet
	EXPECT_FALSE(bomb.has_exploded());
	EXPECT_EQ(bomb.get().hitbox().size, constants::player_projectile::BOMB_SIZE);
	
	sf::Vector2f init_pos = bomb.get().hitbox().position;

	// move
	bomb.update_move(FIXED_DELTA_60);

	// bomb should be moving
	EXPECT_LT(bomb.get().hitbox().position.y, init_pos.y);
	EXPECT_EQ(bomb.get().hitbox().position.x, init_pos.x);

	// explode
	bomb.explode();
	EXPECT_TRUE(bomb.has_exploded());
	EXPECT_EQ(bomb.get().hitbox().size, constants::player_projectile::EXPLOSION_SIZE);

	init_pos = bomb.get().hitbox().position;

	// move
	bomb.update_move(FIXED_DELTA_60);

	// bomb should not be moving when exploded
	EXPECT_EQ(bomb.get().hitbox().position.y, init_pos.y);
	EXPECT_EQ(bomb.get().hitbox().position.x, init_pos.x);

	// check if explosion gets despawned after some time
	constexpr size_t LIFETIME_UPDATES = 
		constants::upgrades::EXPLOSION_TIME / FIXED_DELTA_60 +
		2; // add a bit of leniency

	for (size_t i = 0; i < LIFETIME_UPDATES; i++)
	{
		bomb.update_lifetime(FIXED_DELTA_60, controls);
		controls.execute_requests();
	}

	EXPECT_EQ(controls.count<BombControl>(), 0);
}