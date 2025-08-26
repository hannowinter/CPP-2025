#include "Layer.hpp"

// Creates new layer in window.
Layer::Layer(sf::RenderWindow &window) :
    window{ window },
    target{ window.getSize() },
    sprite{ target.getTexture() }
{
    target.display();
    if (!target.resize(window.getSize()))
        throw std::runtime_error("undefined window size");
}

// Adds entity to layer.
void Layer::add_to_layer(const sf::Drawable &drawable) 
{
    target.draw(drawable);
}

// Draws layer.
void Layer::draw() 
{
    target.display();
    window.draw(sprite);
}

// Clears layer and displays only black.
void Layer::clear() 
{
    target.clear({ 0,0,0,0 });
}

// Sets view on layer.
void Layer::set_view(const sf::View &view) {
    target.setView(view);
}
