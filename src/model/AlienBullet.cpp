#include "AlienBullet.hpp"
#include "Constants.hpp"

AlienBullet::AlienBullet(sf::Vector2f position) :
	m_position{ position }
{

}

sf::FloatRect AlienBullet::hitbox() const
{
	return {
		m_position,
		constants::alien_bullet::SIZE
	};
}

void AlienBullet::move_down(float delta)
{
	m_position.y += constants::alien_bullet::MOVE_SPEED * delta;
}
