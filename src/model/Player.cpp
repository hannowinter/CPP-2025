#include "Player.hpp"
#include "Constants.hpp"

Player::Player(sf::Vector2f position) :
	m_position{ position }
{

}

sf::FloatRect Player::hitbox() const
{
	return {
		m_position,
		constants::player::SIZE
	};
}

void Player::move(Direction direction, float delta)
{
	switch (direction)
	{
	case LEFT:
		m_position.x -= constants::player::MOVE_SPEED * delta;
		break;
	case RIGHT:
		m_position.x += constants::player::MOVE_SPEED * delta;
		break;
	}
	m_position.x = std::clamp(
		m_position.x, 
		constants::PADDING,
		constants::VIEW_WIDTH - constants::player::SIZE.x - constants::PADDING
	);
}