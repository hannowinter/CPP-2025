#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include "Layer.hpp"

// Types of layers
enum class LayerID
{
	// reorder these in order to change draw order
	BACKGROUND,
	EFFECTS,
	ACTORS,
	HUD,

	COUNT
};

// Class managing all layers
class LayerManager
{
	// Array of all layers
	std::array<Layer, static_cast<size_t>(LayerID::COUNT)> m_layers;

	// Creates LayerManager.
	template <size_t... Is>
	LayerManager(std::index_sequence<Is...>, sf::RenderWindow& window) :
		m_layers{ (Is, Layer{ window })... } // use a little trick to be able to construct all array elements at once
	{

	}

public:
	// Creates LayerManager.
	LayerManager(sf::RenderWindow& window);

	// Gets Layer of specific type.
	Layer& get(LayerID layerId);

	// Sets view for all layers.
	void set_view(const sf::View& view);

	// Clears all layers.
	void clear();

	// Draws all layers.
	void draw();
};

#endif