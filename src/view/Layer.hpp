#ifndef LAYER_H
#define LAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>

// Class modeling view layers
class Layer 
{
public:
    // Creates a new layer in a given window.
    Layer(sf::RenderWindow &window);

    // Adds entity to layer.
    void add_to_layer(const sf::Drawable &drawable);

    // Draws layer.
    void draw();

    // Clears layer.
    void clear();

    // Sets view on scene.
    void set_view(const sf::View &view);

private:
    // Parent window
    sf::RenderWindow &window;

    // Buffer with all entities
    sf::RenderTexture target;

    // Graphics generate from RenderTexture
    sf::Sprite sprite;
};

#endif