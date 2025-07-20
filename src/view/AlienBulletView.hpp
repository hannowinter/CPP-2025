#ifndef ALIENBULLETVIEW_H
#define ALIENBULLETVIEW_H

#include <SFML/Graphics.hpp>

#include "Layer.hpp"
#include "../model/AlienBullet.hpp"

class AlienBulletView
{
public:
	AlienBulletView();

	void update(float delta);
	void draw(Layer& layer, const AlienBullet& bullet);

private:
	sf::Sprite m_sprite;
	float m_animation_timer = 0.0f;
	bool m_flipped = false;
};

#endif