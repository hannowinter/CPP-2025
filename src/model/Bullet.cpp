#include "Bullet.hpp"
#include "Constants.hpp"

Bullet::Bullet(sf::Vector2f position) :
	m_position{ position }
{

}

sf::FloatRect Bullet::hitbox() const
{
	return {
		m_position,
		constants::BULLET_SIZE
	};
}

void Bullet::move_up(float delta)
{
	m_position.y -= constants::BULLET_MOVE_SPEED * delta;
}