#include "Alien.hpp"
#include "Constants.hpp"

Alien::Alien(sf::Vector2f grid_origin, size_t column, size_t row) :
	m_column{ column },
	m_row{ row }
{
	align_grid(grid_origin);
}

sf::FloatRect Alien::hitbox() const
{
	return {
		m_position,
		constants::ALIEN_SIZE
	};
}

size_t Alien::column() const
{
	return m_column;
}

size_t Alien::row() const
{
	return m_row;
}

void Alien::align_grid(sf::Vector2f grid_origin)
{
	m_position = grid_origin + sf::Vector2f{ (float)m_column, (float)m_row }.
		componentWiseMul(constants::ALIEN_SIZE + constants::ALIEN_SPACING);
}