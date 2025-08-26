#include "AlienBullet.hpp"
#include "Constants.hpp"
#include "../Util.hpp"

// Initializes new bullet at shooting position.
AlienBullet::AlienBullet(sf::Vector2f position) :
	m_position{ position }
{

}

// Gets hitbox of bullet.
sf::FloatRect AlienBullet::hitbox() const
{
	return {
		m_position,
		constants::alien_bullet::SIZE
	};
}

// Increments vertical positon of bullet.
void AlienBullet::move_down(float delta)
{
	// Calculate offset from speed and amount of time that has passed
	m_position.y += constants::alien_bullet::MOVE_SPEED * delta;
}

bool AlienBullet::outside_view() const
{
	return !overlaps(constants::VIEW_RECT, hitbox());
}
