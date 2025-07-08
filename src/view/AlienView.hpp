#ifndef ALIENVIEW_H
#define ALIENVIEW_H

#include <SFML/Graphics.hpp>

#include "AssetLoader.hpp"
#include "Layer.hpp"
#include "../model/Alien.hpp"

class AlienView
{
public:
	AlienView();

	void draw(Layer& layer, const Alien& alien);

private:
	sf::Sprite m_sprite;
};

#endif