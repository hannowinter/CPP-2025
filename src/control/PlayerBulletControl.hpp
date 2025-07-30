#ifndef PLAYERBULLETCONTROL_H
#define PLAYERBULLETCONTROL_H

#include <SFML/Graphics.hpp>

#include "Controls.hpp"
#include "../model/PlayerBullet.hpp"
#include "../view/Layer.hpp"
#include "../view/PlayerBulletView.hpp"

// Controller for bullet shot by player
class PlayerBulletControl : public Control
{
public:
	// Create Bullet shot by player at position
	PlayerBulletControl(sf::Vector2f position);

	// Abstract methods of parent class
	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	// Get reference to Bullet
	const PlayerBullet& get() const;
	PlayerBullet& get();

private:
	// Controlled Bullet
	PlayerBullet m_bullet;

	// Controlled BulletView
	PlayerBulletView m_bullet_view;
};

#endif