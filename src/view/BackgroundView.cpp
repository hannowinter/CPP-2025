#include "BackgroundView.hpp"
#include "../model/Constants.hpp"
#include <numbers>

// Generates the tile.
sf::Texture generate_tile()
{
	std::mt19937 random;

	sf::Image tile{ { constants::background::TILE_SIZE, constants::background::TILE_SIZE }, { 0, 0, 0, 255 } };
	
	std::uniform_int_distribution<size_t> dist{ 0, constants::background::TILE_SIZE - 1 };

	for (size_t i = 0; i < constants::background::STAR_COUNT; i++)
	{
		tile.setPixel({ (uint32_t)dist(random), (uint32_t)dist(random) }, { 255, 255, 255, 255 });
	}

	return sf::Texture{ tile };
}

BackgroundView::BackgroundView() :
	m_tile_texture{ generate_tile() },
	m_tile{ m_tile_texture }
{

}

void BackgroundView::reset_direction(std::mt19937& random) 
{
	m_direction =
		std::uniform_real_distribution<float>{ 
			-std::numbers::pi_v<float>, 
			std::numbers::pi_v<float>
	    }(random);
}

void BackgroundView::update(float delta, float intensity)
{
	m_offset +=
		delta * intensity * constants::background::BASE_SPEED *
		sf::Vector2f{
			std::cos(m_direction),
			std::sin(m_direction)
		};

	// keep the offset in the range [-TILE_SIZE, 0),
	// wrap around if necessary

	while (m_offset.x >= 0.0f)
		m_offset.x -= constants::background::TILE_SIZE;
	while (m_offset.x < -(float)constants::background::TILE_SIZE)
		m_offset.x += constants::background::TILE_SIZE;
	while (m_offset.y >= 0.0f)
		m_offset.y -= constants::background::TILE_SIZE;
	while (m_offset.y < -(float)constants::background::TILE_SIZE)
		m_offset.y += constants::background::TILE_SIZE;
}

void BackgroundView::draw(Layer& layer)
{
	for (float y = m_offset.y; y < constants::VIEW_HEIGHT; y += constants::background::TILE_SIZE)
	{
		for (float x = m_offset.x; x < constants::VIEW_WIDTH; x += constants::background::TILE_SIZE)
		{
			m_tile.setPosition({ x, y });
			layer.add_to_layer(m_tile);
		}
	}
}

float BackgroundView::get_direction() const
{
	return m_direction;
}

sf::Vector2f BackgroundView::get_offset() const
{
	return m_offset;
}