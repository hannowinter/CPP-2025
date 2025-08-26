#include "Player.hpp"
#include "Constants.hpp"

// Creates player character at a specific position.
Player::Player(sf::Vector2f position) :
	m_position{ position }
{

}

// Gets hitbox of player.
sf::FloatRect Player::hitbox() const
{
	return {
		m_position,
		constants::player::SIZE
	};
}

// Moves player character.
void Player::move(Direction direction, float delta)
{
	// Calculate offset from player speed and amount of time passed
	switch (direction)
	{
	case LEFT:
		m_position.x -= constants::player::MOVE_SPEED * delta;
		break;
	case RIGHT:
		m_position.x += constants::player::MOVE_SPEED * delta;
		break;
	}
}

void Player::clamp_position()
{
	// Ensure player is still within boundaries
	m_position.x = std::clamp(
		m_position.x,
		constants::PADDING,
		constants::VIEW_WIDTH - constants::player::SIZE.x - constants::PADDING
	);
}