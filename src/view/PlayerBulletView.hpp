#ifndef PLAYERBULLETVIEW_H
#define PLAYERBULLETVIEW_H

#include <SFML/Graphics.hpp>

#include "Layer.hpp"
#include "../model/PlayerBullet.hpp"

class PlayerBulletView
{
public:
	PlayerBulletView();

	void draw(Layer& layer, const PlayerBullet& bullet);

private:
	sf::RectangleShape m_sprite;
};

#endif