#ifndef ALIENBULLET_H
#define ALIENBULLET_H

#include <SFML/Graphics.hpp>

class AlienBullet
{
public:
	AlienBullet(sf::Vector2f position);

	sf::FloatRect hitbox() const;
	void move_down(float delta);

private:
	sf::Vector2f m_position;
};

#endif
