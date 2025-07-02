#include "PlayerView.hpp"

PlayerView::PlayerView() :
	m_sprite{ AssetLoader::get().player }
{

}

void PlayerView::draw(Layer& layer, const Player& player)
{
	m_sprite.setPosition(player.hitbox().position);
	layer.add_to_layer(m_sprite);
}