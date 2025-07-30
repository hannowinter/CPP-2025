#ifndef ALIENBULLETCONTROL_H
#define ALIENBULLETCONTROL_H

#include <SFML/Graphics.hpp>

#include "Controls.hpp"
#include "../model/AlienBullet.hpp"
#include "../view/LayerManager.hpp"
#include "../view/AlienBulletView.hpp"

// Controller for Bullet shot by alien
class AlienBulletControl : public Control
{
public:
	// Create BulletControl at position
	AlienBulletControl(sf::Vector2f position);

	// Abstract methods of parent class
	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	// Get reference to controlled bullet
	const AlienBullet& get() const;
	AlienBullet& get();

private:
	// Controlled Bullet
	AlienBullet m_bullet;

	// Controlled view
	AlienBulletView m_bullet_view;
};

#endif