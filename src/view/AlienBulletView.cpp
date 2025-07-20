#include "AlienBulletView.hpp"
#include "AssetLoader.hpp"
#include "../model/Constants.hpp"

constexpr float ANIMATION_LENGTH = 0.1f;

AlienBulletView::AlienBulletView() :
	m_sprite{ AssetLoader::get().alien_bullet }
{

}

void AlienBulletView::update(float delta)
{
	m_animation_timer += delta;
	if (m_animation_timer >= ANIMATION_LENGTH)
	{
		m_animation_timer = 0.0f;
		m_flipped = !m_flipped;
	}
}

void AlienBulletView::draw(Layer& layer, const AlienBullet& bullet)
{
	m_sprite.setPosition(bullet.hitbox().position);
	m_sprite.setOrigin(m_sprite.getTextureRect().size.operator sf::Vector2f() / 2.0f);
	m_sprite.setScale(sf::Vector2f{ m_flipped ? -1.0f : 1.0f, 1.0f });
	layer.add_to_layer(m_sprite);
}