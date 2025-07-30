#ifndef ALIEN_H
#define ALIEN_H

#include <SFML/Graphics.hpp>

// Class to model a single enemy alien
class Alien
{
public:

	// There are three different kinds of aliens identified by their color
	enum Variant
	{
		GREEN, YELLOW, RED
	};

	// Create new alien
	Alien(Variant variant, sf::Vector2f grid_origin, size_t column, size_t row);

	// Get alien hitbox
	sf::FloatRect hitbox() const;

	// Get type of alien
	Variant variant() const;

	// Get / Set position of alien within grid of all aliens
	size_t column() const;
	size_t row() const;
	void set_column_row(size_t column, size_t row);

	// Get / Set absolute position of alien
	sf::Vector2f grid_offset() const;
	void set_position(sf::Vector2f position);

private:
	// Attributes for position and type of alien
	Variant m_variant;
	sf::Vector2f m_position;
	size_t m_column;
	size_t m_row;
};

#endif
