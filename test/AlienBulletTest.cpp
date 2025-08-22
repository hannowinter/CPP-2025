#include "TestCommon.hpp"
#include "../src/control/AlienBulletControl.hpp"

// fixture for AlienBullet model related testing
// contains 3 AlienBullet instances
struct AlienBulletTest : public testing::Test
{
protected:
	constexpr static sf::Vector2f POSITION_1{ 0.0f, 200.0f };
	constexpr static sf::Vector2f POSITION_2{ 20.0f, 200.0f };
	constexpr static sf::Vector2f POSITION_3{ 500.0f, 200.0f };

	constexpr static sf::Vector2f POSITION_ALMOST_OUTSIDE{ 200.0f, (float)constants::VIEW_HEIGHT - EPS };
	constexpr static sf::Vector2f POSITION_OUTSIDE{ 200.0f, (float)constants::VIEW_HEIGHT + EPS };

	AlienBullet alien_bullet1{ POSITION_1 };
	AlienBullet alien_bullet2{ POSITION_2 };
	AlienBullet alien_bullet3{ POSITION_3 };

	AlienBullet alien_bullet_almost_outside{ POSITION_ALMOST_OUTSIDE };
	AlienBullet alien_bullet_outside{ POSITION_OUTSIDE };
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

TEST_F(AlienBulletTest, alienBulletOutsideView)
{
	EXPECT_FALSE(alien_bullet_almost_outside.outside_view());
	EXPECT_TRUE(alien_bullet_outside.outside_view());

	alien_bullet_outside.move_down(FIXED_DELTA_60);

	// should still be outside
	EXPECT_TRUE(alien_bullet_outside.outside_view());
}