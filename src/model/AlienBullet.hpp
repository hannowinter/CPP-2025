#ifndef ALIENBULLET_H
#define ALIENBULLET_H

#include <SFML/Graphics.hpp>

// Class modelling a bullet shot by an alien
class AlienBullet
{
public:
	// Creates new bullet.
	AlienBullet(sf::Vector2f position);

	// Gets hitbox of bullet.
	sf::FloatRect hitbox() const;

	// Makes bullet move further down.
	void move_down(float delta);

	// Checks if alien bullet is outside of view.
	bool outside_view() const;

private:
	// Bullet position
	sf::Vector2f m_position;
};

#endif
