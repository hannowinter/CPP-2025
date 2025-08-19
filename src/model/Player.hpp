#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

// Class modelling player character
class Player
{
public:
	// Player can move either left or right
	enum Direction
	{
		LEFT, RIGHT
	};

	// Create player at position
	Player(sf::Vector2f position);

	// Get hitbox of player
	sf::FloatRect hitbox() const;

	// Move player
	void move(Direction direction, float delta);

	// Ensure the player is always at least "constants::PADDING" away from the view's border
	void clamp_position();

private:
	// Position of player
	sf::Vector2f m_position;
};

#endif