#include "AlienView.hpp"

constexpr float ANIMATION_LENGTH = 0.5f;

// Gets texture for specific variant and animation state.
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

// Creates AlienView and load texture.
AlienView::AlienView(Alien::Variant variant) :
	m_sprite{ get_texture(variant, 0) }
{

}

// Updates animation based on elapsed time.
void AlienView::update(float delta)
{
	// Check if animation state has to be changed
	m_animation_timer += delta;
	if (m_animation_timer >= ANIMATION_LENGTH)
	{
		m_animation_timer = 0.0f;
		m_sprite_index = (m_sprite_index + 1) % 2;
	}
}

// Draws view.
void AlienView::draw(Layer& layer, const Alien& alien)
{
	// Set texture / position and add to layer
	m_sprite.setTexture(get_texture(alien.variant(), m_sprite_index), true);
	m_sprite.setPosition(alien.hitbox().position);
	layer.add_to_layer(m_sprite);
}

// Randomizes animation by setting a random animation time.
void AlienView::randomize_animation(std::mt19937& random)
{
	m_animation_timer = 
		std::uniform_real_distribution<float>{ 0.0f, ANIMATION_LENGTH }(random);
}