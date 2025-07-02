#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet
{
public:
	Bullet(sf::Vector2f position);

	sf::FloatRect hitbox() const;
	void move_up(float delta);

private:
	sf::Vector2f m_position;
};

#endif
