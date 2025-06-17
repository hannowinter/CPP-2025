#ifndef HELLOWORLDCONTROL_H
#define HELLOWOLRDCONTROL_H

#include <SFML/Graphics.hpp>

#include "../view/Layer.hpp"

class HelloWorldControl {
public:
	HelloWorldControl(Layer& layer);

	void move(sf::Vector2f offset);

	void draw();
	
private:
	sf::Font font;
	sf::Text text;

	Layer& layer;
};

#endif