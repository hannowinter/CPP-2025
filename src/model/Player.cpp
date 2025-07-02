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
		constants::PLAYER_SIZE
	};
}

void Player::move(Direction direction, float delta)
{
	switch (direction)
	{
	case LEFT:
		m_position.x -= constants::PLAYER_MOVE_SPEED * delta;
		break;
	case RIGHT:
		m_position.x += constants::PLAYER_MOVE_SPEED * delta;
		break;
	}
	m_position.x = std::clamp(
		m_position.x, 
		constants::PADDING,
		constants::VIEW_WIDTH - constants::PLAYER_SIZE.x - constants::PADDING
	);
}