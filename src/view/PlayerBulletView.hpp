#ifndef PLAYERBULLETVIEW_H
#define PLAYERBULLETVIEW_H

#include <SFML/Graphics.hpp>

#include "Layer.hpp"
#include "../model/PlayerBullet.hpp"

// View for bullets of player
class PlayerBulletView
{
public:
	// Create bullet view
	PlayerBulletView();

	// Draw bullet view
	void draw(Layer& layer, const PlayerBullet& bullet);

private:
	// Non-animated texture
	sf::RectangleShape m_sprite;
};

#endif