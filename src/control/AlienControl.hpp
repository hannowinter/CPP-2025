#ifndef ALIENCONTROL_H
#define ALIENCONTROL_H

#include <SFML/Graphics.hpp>

#include "Controls.hpp"
#include "../model/Alien.hpp"
#include "../view/Layer.hpp"
#include "../view/AlienView.hpp"

class AlienControl : public Control
{
public:
	AlienControl(sf::Vector2f grid_origin, size_t column, size_t row);

	void update(const UpdateState& state) override;
	void draw(Layer& layer) override;

	Alien& get();
	const Alien& get() const;

private:
	Alien m_alien;
	AlienView m_alien_view;
};

#endif