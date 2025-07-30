#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include "Layer.hpp"

// Types of layeres
enum class LayerID
{
	// reorder these in order to change draw order
	ACTORS,
	HUD,
	COUNT
};

// Class managing all layers
class LayerManager
{
	// Array of all layers
	std::array<Layer, static_cast<size_t>(LayerID::COUNT)> m_layers;

	// Create LayerManager
	template <size_t... Is>
	LayerManager(std::index_sequence<Is...>, sf::RenderWindow& window) :
		m_layers{ (Is, Layer{ window })... } // use a little trick to be able to construct all array elements at once
	{

	}

public:
	// Create LayerManager
	LayerManager(sf::RenderWindow& window);

	// Get Layer of specific type
	Layer& get(LayerID layerId);

	// Set view for all layers
	void set_view(const sf::View& view);

	// Clear all layers
	void clear();

	// Draw all layers
	void draw();
};

#endif