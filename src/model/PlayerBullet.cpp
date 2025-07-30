#include "PlayerBullet.hpp"
#include "Constants.hpp"

// Create new bullet at position of shooting
PlayerBullet::PlayerBullet(sf::Vector2f position) :
	m_position{ position }
{

}

// Get hitbox of bullet
sf::FloatRect PlayerBullet::hitbox() const
{
	return {
		m_position,
		constants::player_bullet::SIZE
	};
}

// Move bullet
void PlayerBullet::move_up(float delta)
{
	// Calculate offset from bullet speed and amount of time passed
	m_position.y -= constants::player_bullet::MOVE_SPEED * delta;
}
