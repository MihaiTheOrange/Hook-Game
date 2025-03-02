#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"

/*
	Clasa va fi engine-ul jocului
*/

class Game
{
private:
	//Variabile
	sf::RenderWindow* window;
	sf::VideoMode videoMode;

	sf::Texture backgroundTexture;
	sf::Sprite *backgroundSprite;

	Player player;

	//Metode private
	void initVariables();
	void initWindow();
	void initBackground();

public:
	// Constructor / Desctructor
	Game();
	virtual ~Game();

	//Functii
	void pollEvents();
	void update(float dt);
	void render();

	bool running() const;
};

