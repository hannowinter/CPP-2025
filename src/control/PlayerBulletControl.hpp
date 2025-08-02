#ifndef PLAYERBULLETCONTROL_H
#define PLAYERBULLETCONTROL_H

#include <SFML/Graphics.hpp>

#include "PlayerProjectileControl.hpp"

// Controller for bullet shot by player
class PlayerBulletControl : public PlayerProjectileControl
{
public:

	// Create Bullet shot by player at position
	PlayerBulletControl(sf::Vector2f position);
};

#endif