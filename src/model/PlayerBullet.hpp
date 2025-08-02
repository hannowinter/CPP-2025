#ifndef PLAYERBULLET_H
#define PLAYERBULLET_H

#include <SFML/Graphics.hpp>

#include "PlayerProjectile.hpp"

// Class modelling a bullet shot by the player
class PlayerBullet : public PlayerProjectile
{
public:
	// Create player bullet
	PlayerBullet(sf::Vector2f position);

	// Get hitbox of bullet
	sf::FloatRect hitbox() const override;
};

#endif
