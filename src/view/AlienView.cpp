#include "AlienView.hpp"

AlienView::AlienView() :
	m_sprite{ AssetLoader::get().yellow }
{

}

void AlienView::draw(Layer& layer, const Alien& alien)
{
	m_sprite.setPosition(alien.hitbox().position);
	layer.add_to_layer(m_sprite);
}