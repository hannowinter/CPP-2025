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

private:
	// Texture of player
	sf::Sprite m_sprite;
};

#endif