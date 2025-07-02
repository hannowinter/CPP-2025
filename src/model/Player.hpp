#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player
{
public:
	enum Direction
	{
		LEFT, RIGHT
	};

	Player(sf::Vector2f position);

	sf::FloatRect hitbox() const;
	void move(Direction direction, float delta);

private:
	sf::Vector2f m_position;
};

#endif