#include "PlayerBullet.hpp"
#include "Constants.hpp"

PlayerBullet::PlayerBullet(sf::Vector2f position) :
	m_position{ position }
{

}

sf::FloatRect PlayerBullet::hitbox() const
{
	return {
		m_position,
		constants::player_bullet::SIZE
	};
}

void PlayerBullet::move_up(float delta)
{
	m_position.y -= constants::player_bullet::MOVE_SPEED * delta;
}
