#include "PlayerBullet.hpp"
#include "Constants.hpp"

// Create new bullet at position of shooting
PlayerBullet::PlayerBullet(sf::Vector2f position) :
	PlayerProjectile(position)
{

}

// Get hitbox of bullet
sf::FloatRect PlayerBullet::hitbox() const
{
	return {
		m_position,
		constants::player_bullet::BULLET_SIZE
	};
}