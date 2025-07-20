#ifndef PLAYERBULLET_H
#define PLAYERBULLET_H

#include <SFML/Graphics.hpp>

class PlayerBullet
{
public:
	PlayerBullet(sf::Vector2f position);

	sf::FloatRect hitbox() const;
	void move_up(float delta);

private:
	sf::Vector2f m_position;
};

#endif
