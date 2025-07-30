#ifndef ALIENBULLET_H
#define ALIENBULLET_H

#include <SFML/Graphics.hpp>

// Class modelling a bullet shot by an alien
class AlienBullet
{
public:
	// Create new bullet
	AlienBullet(sf::Vector2f position);

	// Get hitbox of bullet
	sf::FloatRect hitbox() const;

	// Make bullet move further down
	void move_down(float delta);

private:
	// Bullet position
	sf::Vector2f m_position;
};

#endif
