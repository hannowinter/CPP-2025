#include "SmokeView.hpp"

#include "AssetLoader.hpp"

SmokeView::SmokeView() :
    m_smoke { AssetLoader::get().smoke }
{

}

// Draws view to screen.
void SmokeView::draw(Layer& layer, const sf::Vector2f position)
{
    m_smoke.setPosition(position);
    layer.add_to_layer(m_smoke);
}