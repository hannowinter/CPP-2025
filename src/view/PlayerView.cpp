#include "PlayerView.hpp"

// Create PlayerView
PlayerView::PlayerView() :
	m_sprite{ AssetLoader::get().player }
{

}

// Draw player
void PlayerView::draw(Layer& layer, const Player& player)
{
	// Set position of texture and add to layer
	m_sprite.setPosition(player.hitbox().position);
	layer.add_to_layer(m_sprite);
}