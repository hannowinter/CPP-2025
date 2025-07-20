#include "Alien.hpp"
#include "Constants.hpp"

Alien::Alien(Variant variant, sf::Vector2f grid_origin, size_t column, size_t row) :
	m_variant{ variant },
	m_column{ column },
	m_row{ row }
{
	m_position = grid_origin + grid_offset();
}

sf::FloatRect Alien::hitbox() const
{
	return {
		m_position,
		constants::alien::SIZE
	};
}

Alien::Variant Alien::variant() const
{
	return m_variant;
}

size_t Alien::column() const
{
	return m_column;
}

size_t Alien::row() const
{
	return m_row;
}

void Alien::set_column_row(size_t column, size_t row)
{
	m_column = column;
	m_row = row;
}

sf::Vector2f Alien::grid_offset() const
{
	return sf::Vector2f{ (float)m_column, (float)m_row }.
		componentWiseMul(constants::alien::SIZE + constants::alien::SPACING);
}

void Alien::set_position(sf::Vector2f position)
{
	m_position = position;
}