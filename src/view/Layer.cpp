#include "Layer.hpp"

// Create new layer in window
Layer::Layer(sf::RenderWindow &window) :
    window{ window },
    target{ window.getSize() },
    sprite{ target.getTexture() }
{
    target.display();
    if (!target.resize(window.getSize()))
        throw std::runtime_error("undefined window size");
}

// Add entity to layer
void Layer::add_to_layer(const sf::Drawable &drawable) 
{
    target.draw(drawable);
}

// Draw layer
void Layer::draw() 
{
    target.display();
    window.draw(sprite);
}

// Clear layer and display only black
void Layer::clear() 
{
    target.clear({ 0,0,0,0 });
}

// Set view on layer
void Layer::set_view(const sf::View &view) {
    target.setView(view);
}
