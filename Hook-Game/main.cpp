#include "Game.h"
#include <SFML/Graphics.hpp>

// TO DO: 
// de organizat clasa Player(bafta)
// muzica - probabil nu apuc

int main()
{
    sf::Texture texture;
    sf::Sprite sprite(texture);
    Game game;
    sf::Clock clock;
    float dt = 0.f;
    while (game.running())
    {
        dt = clock.getElapsedTime().asSeconds();
        clock.reset();
        clock.start();
        
        //Update
        game.update(dt);

        //Render
        game.render();
    }

    return 0;
}