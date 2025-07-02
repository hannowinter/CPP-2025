#include "BulletView.hpp"

BulletView::BulletView()
{
	m_sprite.setFillColor(sf::Color::White);
}

void BulletView::draw(Layer& layer, const Bullet& bullet)
{
	m_sprite.setPosition(bullet.hitbox().position);
	m_sprite.setSize(bullet.hitbox().size);
	layer.add_to_layer(m_sprite);
}