#ifndef BULLETVIEW_H
#define BULLETVIEW_H

#include <SFML/Graphics.hpp>

#include "Layer.hpp"
#include "../model/Bullet.hpp"

class BulletView
{
public:
	BulletView();

	void draw(Layer& layer, const Bullet& bullet);

private:
	sf::RectangleShape m_sprite;
};

#endif