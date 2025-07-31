#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include <SFML/Graphics.hpp>

#include "AssetLoader.hpp"
#include "Layer.hpp"
#include "../model/Player.hpp"

// View for Player
class PlayerView
{
public:
	// Create PlayerView
	PlayerView();

	// Draw PlayerView
	void draw(Layer& layer, const Player& player);

	// Update view based on elapsed time
	void update(float delta);

	// Run hit animation
	void hit_animation();

private:
	// Texture of player
	sf::Sprite m_sprite;

	// Boolean indicating whether player has been hit
	bool m_hit = false;

	// Time remaining in animation and phase of animation
	float m_animation_timer = 0.0f;
	size_t m_animation_phase = 0;
};

#endif