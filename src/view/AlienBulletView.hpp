#ifndef ALIENBULLETVIEW_H
#define ALIENBULLETVIEW_H

#include <SFML/Graphics.hpp>

#include "Layer.hpp"
#include "../model/AlienBullet.hpp"

// View class for alien bullets
class AlienBulletView
{
public:
	// Create view
	AlienBulletView();

	// Update view based on elapsed time
	void update(float delta);

	// Draw view
	void draw(Layer& layer, const AlienBullet& bullet);

private:
	// Texture to be drawn
	sf::Sprite m_sprite;

	// Timer for animation
	float m_animation_timer = 0.0f;

	// Store whether the regular or the flipped texture has to be shown
	bool m_flipped = false;
};

#endif