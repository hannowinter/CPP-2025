#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include "Layer.hpp"

enum class LayerID
{
	ACTORS,
	HUD,

	COUNT
};

class LayerManager
{
	std::array<Layer, static_cast<size_t>(LayerID::COUNT)> m_layers;

	template <size_t... Is>
	LayerManager(std::index_sequence<Is...>, sf::RenderWindow& window) :
		m_layers{ (Is, Layer{ window })... } // use a little trick to be able to construct all array elements at once
	{

	}

public:
	LayerManager(sf::RenderWindow& window);

	Layer& get(LayerID layerId);
	void set_view(const sf::View& view);
	void clear();
	void draw();
};

#endif