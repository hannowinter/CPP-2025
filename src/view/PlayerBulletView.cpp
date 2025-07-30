#include "PlayerBulletView.hpp"

// Create PlayerBulletView
PlayerBulletView::PlayerBulletView()
{
	// PlayerBullets are white rectangles
	m_sprite.setFillColor(sf::Color::White);
}

// Draw PlayerBullet
void PlayerBulletView::draw(Layer& layer, const PlayerBullet& bullet)
{
	// Set position / size of texture and add to layer
	m_sprite.setPosition(bullet.hitbox().position);
	m_sprite.setSize(bullet.hitbox().size);
	layer.add_to_layer(m_sprite);
}