#ifndef ALIENVIEW_H
#define ALIENVIEW_H

#include <SFML/Graphics.hpp>
#include <random>

#include "AssetLoader.hpp"
#include "Layer.hpp"
#include "../model/Alien.hpp"

// View for aliens
class AlienView
{
public:
	// Create AlienView of specific variant
	AlienView(Alien::Variant variant);

	// Update view based on elapsed time
	void update(float delta);

	// Draw the view
	void draw(Layer& layer, const Alien& alien);

	// Randomize the animation of the alien
	void randomize_animation(std::mt19937& random);

private:
	// Texture of the alien
	sf::Sprite m_sprite;

	// Current animation state
	size_t m_sprite_index = 0;

	// Timer for animation
	float m_animation_timer = 0.0f;
};

#endif