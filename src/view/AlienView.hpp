#ifndef ALIENVIEW_H
#define ALIENVIEW_H

#include <SFML/Graphics.hpp>
#include <random>

#include "AssetLoader.hpp"
#include "Layer.hpp"
#include "../model/Alien.hpp"

class AlienView
{
public:
	AlienView(Alien::Variant variant);

	void update(float delta);
	void draw(Layer& layer, const Alien& alien);

	void randomize_animation(std::mt19937& random);

private:
	sf::Sprite m_sprite;
	size_t m_sprite_index = 0;
	float m_animation_timer = 0.0f;
};

#endif