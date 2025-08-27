#include "PlayerView.hpp"

constexpr float ANIMATION_LENGTH = 0.15f;

// Creates PlayerView.
PlayerView::PlayerView() :
	m_sprite{ AssetLoader::get().player }
{

}

// Updates view based on elapsed time.
void PlayerView::update(float delta)
{
	// Only animate player when hit
	if (m_hit)
	{
		// Increment animation timer
		m_animation_timer += delta;

		// Check if next phase is reached
		if (m_animation_timer >= ANIMATION_LENGTH)
		{
			m_animation_timer = 0.0f;
			m_animation_phase = (m_animation_phase + 1) % 5;

			if (m_animation_phase == 0)
				m_hit = false;
		}

		// Use the phase parity to choose texture: even phases normal, odd phases hit
		bool is_normal_phase = (m_animation_phase % 2) == 0;
		m_sprite.setTexture(is_normal_phase ? AssetLoader::get().player : AssetLoader::get().player_hit);
	}
}

// Draws player.
void PlayerView::draw(Layer& layer, const Player& player)
{
	// Set position of texture and add to layer
	m_sprite.setPosition(player.hitbox().position);
	layer.add_to_layer(m_sprite);
}

void PlayerView::hit_animation()
{
	m_hit = true;
}
