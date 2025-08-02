#include "PlayerBulletControl.hpp"

#include "../model/PlayerBullet.hpp"
#include "../view/PlayerBulletView.hpp"

// Create BulletController for Bullet at position
PlayerBulletControl::PlayerBulletControl(sf::Vector2f position) :
	PlayerProjectileControl(std::make_unique<PlayerBullet>(position), std::make_unique<PlayerBulletView>())
{

}