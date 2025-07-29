#include "LayerManager.hpp"

LayerManager::LayerManager(sf::RenderWindow& window) :
	LayerManager{ std::make_index_sequence<static_cast<size_t>(LayerID::COUNT)>{}, window }
{

}

Layer& LayerManager::get(LayerID layer)
{
	return m_layers[static_cast<size_t>(layer)];
}

void LayerManager::set_view(const sf::View& view)
{
	for (Layer& layer : m_layers)
		layer.set_view(view);
}

void LayerManager::clear()
{
	for (Layer& layer : m_layers)
		layer.clear();
}

void LayerManager::draw()
{
	for (Layer& layer : m_layers)
		layer.draw();
}