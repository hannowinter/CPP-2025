#ifndef ALIENBULLETCONTROL_H
#define ALIENBULLETCONTROL_H

#include <SFML/Graphics.hpp>

#include "Controls.hpp"
#include "../model/AlienBullet.hpp"
#include "../view/LayerManager.hpp"
#include "../view/AlienBulletView.hpp"

class AlienBulletControl : public Control
{
public:
	AlienBulletControl(sf::Vector2f position);

	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	const AlienBullet& get() const;
	AlienBullet& get();

private:
	AlienBullet m_bullet;
	AlienBulletView m_bullet_view;
};

#endif