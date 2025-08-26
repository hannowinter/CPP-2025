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

	// Creates new alien.
	Alien(Variant variant, sf::Vector2f grid_origin, size_t column, size_t row);

	// Gets the alien's hitbox.
	sf::FloatRect hitbox() const;

	// Gets the alien's variant.
	Variant variant() const;

	// Gets / sets position of alien within grid of all aliens.
	size_t column() const;
	size_t row() const;
	void set_column_row(size_t column, size_t row);

	// Gets offset of alien within grid.
	sf::Vector2f grid_offset() const;

	// Sets the alien's absolute position.
	void set_position(sf::Vector2f position);

	// Remember if alien has already hit player
	bool has_hit_player = false;

private:
	// Attributes for position and type of alien
	Variant m_variant;
	sf::Vector2f m_position;
	size_t m_column;
	size_t m_row;
};

#endif
