#include "LayerManager.hpp"

// Creates LayerManager.
LayerManager::LayerManager(sf::RenderWindow& window) :
	LayerManager{ std::make_index_sequence<static_cast<size_t>(LayerID::COUNT)>{}, window }
{

}

// Gets all layers of specific type.
Layer& LayerManager::get(LayerID layer)
{
	return m_layers[static_cast<size_t>(layer)];
}

// Sets view for all layers.
void LayerManager::set_view(const sf::View& view)
{
	for (Layer& layer : m_layers)
		layer.set_view(view);
}

// Clears all layers.
void LayerManager::clear()
{
	for (Layer& layer : m_layers)
		layer.clear();
}

// Draws all layers.
void LayerManager::draw()
{
	for (Layer& layer : m_layers)
		layer.draw();
}