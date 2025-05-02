#include "Game.h"
#include <SFML/Graphics.hpp>

//TO DO: 
// scapa de scalari
// de organizat clasa Player(bafta)
// hook - oprire movement la max length, climbing, posibil swing(nu prea cred)
// muzica 

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