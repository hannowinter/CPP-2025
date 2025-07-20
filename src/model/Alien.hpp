#ifndef ALIEN_H
#define ALIEN_H

#include <SFML/Graphics.hpp>

class Alien
{
public:
	enum Variant
	{
		GREEN, YELLOW, RED
	};

	Alien(Variant variant, sf::Vector2f grid_origin, size_t column, size_t row);

	sf::FloatRect hitbox() const;

	Variant variant() const;

	size_t column() const;
	size_t row() const;
	void set_column_row(size_t column, size_t row);

	sf::Vector2f grid_offset() const;
	void set_position(sf::Vector2f position);

private:
	Variant m_variant;
	sf::Vector2f m_position;
	size_t m_column;
	size_t m_row;
};

#endif
