#ifndef ALIENBULLETCONTROL_H
#define ALIENBULLETCONTROL_H

#include <SFML/Graphics.hpp>

#include "Control.hpp"
#include "../model/AlienBullet.hpp"
#include "../view/LayerManager.hpp"
#include "../view/AlienBulletView.hpp"

// Control class for bullets shot by aliens
class AlienBulletControl : public Control
{
public:
	// Creates BulletControl at position.
	AlienBulletControl(sf::Vector2f position);

	// Abstract methods of parent class
	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	// Gets reference to the model object.
	const AlienBullet& get() const;
	AlienBullet& get();

private:
	// Model
	AlienBullet m_bullet;

	// View
	AlienBulletView m_bullet_view;
};

#endif