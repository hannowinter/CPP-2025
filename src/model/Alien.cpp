#include "Alien.hpp"
#include "Constants.hpp"

// Create a new alien wihthin a grid
Alien::Alien(Variant variant, sf::Vector2f grid_origin, size_t column, size_t row) :
	m_variant{ variant },
	m_column{ column },
	m_row{ row }
{
	m_position = grid_origin + grid_offset();
}

// Get hitbox of alien
sf::FloatRect Alien::hitbox() const
{
	return {
		m_position,
		constants::alien::SIZE
	};
}

// Get variant of alien
Alien::Variant Alien::variant() const
{
	return m_variant;
}

// Get column of alien within grid
size_t Alien::column() const
{
	return m_column;
}

// Get row of alien wihtin grid
size_t Alien::row() const
{
	return m_row;
}

// Set column and row of alien
void Alien::set_column_row(size_t column, size_t row)
{
	m_column = column;
	m_row = row;
}

// Get offset of alien wihtin grid
sf::Vector2f Alien::grid_offset() const
{
	// Offset ist calculated from column / row of alien and alien size / spacing in grid
	sf::Vector2f offset = sf::Vector2f{ (float)m_column, (float)m_row }.
		componentWiseMul(constants::alien::SIZE + constants::alien_grid::SPACING);
	//if (m_row % 2 == 1)
		//offset.x = constants::VIEW_WIDTH - offset.x;
	return offset;
}

// Set position of alien
void Alien::set_position(sf::Vector2f position)
{
	m_position = position;
}