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
	//Constante
	const float restartAfter = 5.f;
	
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
	Menu* winScreen;
	Player player;
	Level test_level;

	float restartTimer = 0.f;


	//Metode private
	void initVariables();
	void initWindow();
	void initBackground();
	void initFonts();
	void initMainMenu();
	void initWinScreen();


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

