#ifndef PLAYERBULLETCONTROL_H
#define PLAYERBULLETCONTROL_H

#include <SFML/Graphics.hpp>

#include "Controls.hpp"
#include "../model/PlayerBullet.hpp"
#include "../view/Layer.hpp"
#include "../view/PlayerBulletView.hpp"

class PlayerBulletControl : public Control
{
public:
	PlayerBulletControl(sf::Vector2f position);

	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	const PlayerBullet& get() const;
	PlayerBullet& get();

private:
	PlayerBullet m_bullet;
	PlayerBulletView m_bullet_view;
};

#endif