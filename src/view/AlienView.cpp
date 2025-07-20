#include "AlienView.hpp"

constexpr float ANIMATION_LENGTH = 0.5f;

const sf::Texture& get_texture(Alien::Variant variant, size_t index)
{
	switch (variant)
	{
	case Alien::GREEN:
		return AssetLoader::get().green[index];
	case Alien::YELLOW:
		return AssetLoader::get().yellow[index];
	case Alien::RED:
		return AssetLoader::get().red[index];
	}
}

AlienView::AlienView(Alien::Variant variant) :
	m_sprite{ get_texture(variant, 0) }
{

}

void AlienView::update(float delta)
{
	m_animation_timer += delta;
	if (m_animation_timer >= ANIMATION_LENGTH)
	{
		m_animation_timer = 0.0f;
		m_sprite_index = (m_sprite_index + 1) % 2;
	}
}

void AlienView::draw(Layer& layer, const Alien& alien)
{
	m_sprite.setTexture(get_texture(alien.variant(), m_sprite_index), true);
	m_sprite.setPosition(alien.hitbox().position);
	layer.add_to_layer(m_sprite);
}

void AlienView::randomize_animation(std::mt19937& random)
{
	m_animation_timer = 
		std::uniform_real_distribution<float>{ 0.0f, ANIMATION_LENGTH }(random);
}