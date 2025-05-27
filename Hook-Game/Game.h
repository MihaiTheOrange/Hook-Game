#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Level.h"
#include "Rope.h"
#include "Menu.h"

/*
	Clasa va fi engine-ul jocului
*/

class Game
{
private:
	//Variabile

	enum class gameStates {
		MainMenu,
		Playing,
		Won
	}currentGameState;

	sf::Font font;

	sf::RenderWindow* window;
	sf::VideoMode videoMode;

	sf::View* camera;

	sf::Texture backgroundTexture;
	sf::Sprite *backgroundSprite;

	Menu* mainMenu;
	Player player;
	Level test_level;




	//Metode private
	void initVariables();
	void initWindow();
	void initBackground();
	void initFonts();
	void initMainMenu();


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

