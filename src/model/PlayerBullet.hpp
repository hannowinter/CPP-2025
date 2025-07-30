#ifndef PLAYERBULLET_H
#define PLAYERBULLET_H

#include <SFML/Graphics.hpp>

// Class modelling a bullet shot by the player
class PlayerBullet
{
public:
	// Create player bullet
	PlayerBullet(sf::Vector2f position);

	// Get hitbox of bullet
	sf::FloatRect hitbox() const;

	// Move bullet
	void move_up(float delta);

private:
	// Position of bullet
	sf::Vector2f m_position;
};

#endif
