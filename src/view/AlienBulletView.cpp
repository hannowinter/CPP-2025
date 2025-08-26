#include "AlienBulletView.hpp"
#include "AssetLoader.hpp"
#include "../model/Constants.hpp"

// Creates view for alien bullet.
AlienBulletView::AlienBulletView() :
	m_sprite{ AssetLoader::get().alien_bullet }
{

}

// Updates view based on elapsed time.
void AlienBulletView::update(float delta)
{
	m_animation_timer += delta;
	if (m_animation_timer >= constants::alien_bullet::ANIMATION_LENGTH)
	{
		// Reset timer and flip texture
		m_animation_timer = 0.0f;
		m_flipped = !m_flipped;
	}
}

// Draws view.
void AlienBulletView::draw(Layer& layer, const AlienBullet& bullet)
{
	// Set position for texture
	m_sprite.setPosition(bullet.hitbox().position);
	m_sprite.setOrigin(m_sprite.getTextureRect().size.operator sf::Vector2f() / 2.0f);

	// Flip texture if necessary
	m_sprite.setScale(sf::Vector2f{ m_flipped ? -1.0f : 1.0f, 1.0f });

	// Add texture to layer
	layer.add_to_layer(m_sprite);
}

bool AlienBulletView::is_flipped() const
{
	return m_flipped;
}