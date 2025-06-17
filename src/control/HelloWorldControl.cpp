#include "HelloWorldControl.hpp"

HelloWorldControl::HelloWorldControl(Layer& layer) :
	font("assets/fonts/DejaVuSansMono.ttf"),
	text(font, "Hello World", 30),
	layer(layer) {
	text.setFillColor(sf::Color::Magenta);
	text.setPosition(sf::Vector2f(0.0f, 0.0f));
}

void HelloWorldControl::move(sf::Vector2f offset) {
	text.setPosition(text.getPosition() + offset);
}

void HelloWorldControl::draw() {
	layer.add_to_layer(text);
}