#ifndef ALIEN_H
#define ALIEN_H

#include <SFML/Graphics.hpp>

class Alien
{
public:
	enum Mode
	{
		SHIFT_RIGHT, SHIFT_LEFT, DESCEND
	};

	Alien(sf::Vector2f grid_origin, size_t column, size_t row);

	sf::FloatRect hitbox() const;

	size_t column() const;
	size_t row() const;

	void align_grid(sf::Vector2f grid_origin);

private:
	sf::Vector2f m_position;
	size_t m_column;
	size_t m_row;
};

#endif
