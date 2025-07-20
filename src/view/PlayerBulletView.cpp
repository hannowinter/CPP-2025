#include "PlayerBulletView.hpp"

PlayerBulletView::PlayerBulletView()
{
	m_sprite.setFillColor(sf::Color::White);
}

void PlayerBulletView::draw(Layer& layer, const PlayerBullet& bullet)
{
	m_sprite.setPosition(bullet.hitbox().position);
	m_sprite.setSize(bullet.hitbox().size);
	layer.add_to_layer(m_sprite);
}