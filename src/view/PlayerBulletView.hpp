#ifndef PLAYERBULLETVIEW_H
#define PLAYERBULLETVIEW_H

#include <SFML/Graphics.hpp>

#include "Layer.hpp"
#include "PlayerProjectileView.hpp"
#include "../model/PlayerBullet.hpp"

// View for bullets of player
class PlayerBulletView : public PlayerProjectileView
{
public:

	// Create bullet view
	PlayerBulletView();
};

#endif