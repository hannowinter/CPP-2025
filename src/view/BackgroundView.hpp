#ifndef BACKGROUNDVIEW_H
#define BACKGROUNDVIEW_H

#include "Layer.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <random>

// View for the starry background
class BackgroundView
{
public:
	// Create view
	BackgroundView();

	// Reset the move direction to a random angle
	void reset_direction(std::mt19937& random);

	// Update the offset
	void update(float delta, float intensity);

	// Draw the background
	void draw(Layer& layer);

private:
	// The background reuses the same tile and draws it repeatedly with
	// different offsets to fill the entire screen
	sf::Texture m_tile_texture;
	sf::Sprite m_tile;

	// The offset of the most topleft tile to draw
	sf::Vector2f m_offset;

	// The move direction as an angle
	float m_direction;
};

#endif