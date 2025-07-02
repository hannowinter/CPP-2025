#ifndef LAYER_H
#define LAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>

class Layer 
{
public:   
    Layer(sf::RenderWindow &window);

    void add_to_layer(const sf::Drawable &drawable);

    void draw();

    void clear();

    void set_view(const sf::View &view);

private:
    sf::RenderWindow &window;

    sf::RenderTexture target;

    sf::Sprite sprite;
};

#endif