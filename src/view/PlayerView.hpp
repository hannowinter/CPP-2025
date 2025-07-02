#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include <SFML/Graphics.hpp>

#include "AssetLoader.hpp"
#include "Layer.hpp"
#include "../model/Player.hpp"

class PlayerView
{
public:
	PlayerView();

	void draw(Layer& layer, const Player& player);

private:
	sf::Sprite m_sprite;
};

#endif