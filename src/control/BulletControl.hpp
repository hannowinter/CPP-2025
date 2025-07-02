#ifndef BULLETCONTROL_H
#define BULLETCONTROL_H

#include <SFML/Graphics.hpp>

#include "Actors.hpp"
#include "../model/Bullet.hpp"
#include "../view/Layer.hpp"
#include "../view/BulletView.hpp"

class BulletControl : public ActorControl
{
public:
	BulletControl(sf::Vector2f position);

	void update(const UpdateState& state) override;
	void draw(Layer& layer) override;

private:
	Bullet m_bullet;
	BulletView m_bullet_view;
};

#endif