#include "Game.h"

bool Menu::clicked = false;


//Metode private
void Game::initVariables()
{
	this->window = nullptr;
}

void Game::initWindow()
{
	this->videoMode.size = { 800, 600 };
	this->videoMode.bitsPerPixel = 32;
	this->window = new sf::RenderWindow(this->videoMode, "Hook-Game");
	this->window->setFramerateLimit(60);
	
	float xsize = static_cast<float>(window->getSize().x);
	float ysize = static_cast<float>(window->getSize().y);
}

void Game::initBackground()
{
	if (this->backgroundTexture.loadFromFile("Assets/Backgrounds/background.png"))
	{
		std::cout << "Background loaded!" << std::endl;
		this->backgroundSprite = new sf::Sprite(this->backgroundTexture);
		this->backgroundSprite->setScale(
			{ 
				static_cast<float>(window->getSize().x) / backgroundTexture.getSize().x ,
				static_cast<float>(window->getSize().y) / backgroundTexture.getSize().y 
			}

		);
	}
	else
		std::cout << "Failed to load background" << std::endl;
}

void Game::initFonts()
{
	if(!font.openFromFile("Assets/Fonts/GameFont.ttf"))
	{
		std::cout << "Failed to load font!" << std::endl;
	}
	else
	{
		std::cout << "Font loaded!" << std::endl;
	}
}

void Game::initMainMenu()
{
	this->mainMenu = new Menu(this->font);

	this->mainMenu->addOption("Play", [&]() {
		this->currentGameState = gameStates::Playing;
		this->selectedPlayer->respawn(test_level);
		});

	this->mainMenu->addOption("Select Player", [&]() {
		this->currentGameState = gameStates::SelectPlayer;
		this->selectedPlayer->respawn(test_level);
		});

	this->mainMenu->addOption("Exit", [&]() {
		this->window->close();
		});

	sf::Vector2f halfScreenPos = {250.f, 10.f};
	this->mainMenu->addLabel("HOOK GAME!", 24, halfScreenPos, sf::Color::Red);
}

void Game::initWinScreen()
{
	this->winScreen = new Menu(this->font);
	sf::Vector2f cenPos( this->window->getSize().x/2.f - 192.f, this->window->getSize().y/4.f );
	std::cout << "cenPos" << cenPos.x << " " << cenPos.y << std::endl;
	this->winScreen->addLabel("YOU WON", 48, {100.f, 100.f}, sf::Color::Yellow);
}

void Game::initSelectPlayer()
{
	this->selectPlayerMenu = new Menu(this->font);

	this->selectPlayerMenu->addOption("Mihaela Florea", [&]() {
		this->selectedPlayer = &Mihaela;
		this->selectedPlayer->respawn(test_level);
		});

	this->selectPlayerMenu->addOption("Sebastian Valhenstein", [&]() {
		this->selectedPlayer = &Sebastian;
		this->selectedPlayer->respawn(test_level);
		});

	this->selectPlayerMenu->addOption("Back", [&]() {
		this->currentGameState = gameStates::MainMenu;
		this->selectedPlayer->respawn(test_level);
		});
}



// Constructor / Destructor
Game::Game() : test_level("Assets/Levels/Test_level/test3_1.txt", "Assets/Levels/Test_level/Snow.png", "Assets/Backgrounds/parallax2.png", 
	true, {189, 1}), currentGameState(gameStates::MainMenu)
{
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initMainMenu();
	this->initSelectPlayer();

	this->camera = this->test_level.InitView(*window);
	
	this->initWinScreen();

	this->selectedPlayer = &Mihaela;

	//this->selectedPlayer->respawn(test_level);
}

Game::~Game()
{
	delete this->window;
	delete this->backgroundSprite;
	delete this->mainMenu;
}



// Update / Render / Event polling
void Game::pollEvents()
{
	while (const std::optional event = window->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window->close();
		}
	}
	
}

void Game::update(float dt)
{
	this->pollEvents();

	if (selectedPlayer->checkWinCondition(test_level))
	{
		this->currentGameState = gameStates::Won;
	}

	if (this->currentGameState == gameStates::MainMenu)
	{
		this->camera->setCenter(this->camera->getCenter() + sf::Vector2f(1.f, 0.f));
		this->window->setView(*this->camera);
		this->mainMenu->update(*window, this->camera->getCenter(), this->camera->getSize());
	}

	if (this->currentGameState == gameStates::SelectPlayer)
	{
		this->camera->setCenter(this->camera->getCenter() + sf::Vector2f(1.f, 0.f));
		this->window->setView(*this->camera);
		this->selectPlayerMenu->update(*window, this->camera->getCenter(), this->camera->getSize());
	}

	if (this->currentGameState == gameStates::Won)
	{
		this->camera->setCenter(this->camera->getCenter() + sf::Vector2f(1.f, 0.f));
		this->window->setView(*this->camera);
		this->winScreen->update(*window, this->camera->getCenter(), this->camera->getSize());
		restartTimer += dt;


		if (restartTimer >= restartAfter)
		{
			this->currentGameState = gameStates::MainMenu;
			this->selectedPlayer->setPosition({ 0.f, 0.f });
			this->selectedPlayer->update(dt, test_level, *window);
			this->restartTimer = 0.f;
		}
	}
	if (currentGameState == gameStates::Playing)
	{
		selectedPlayer->update(dt, test_level, *window);
		this->test_level.update(dt, *window, selectedPlayer->getPlayerPosition());
	}
	Menu::clicked = checkForClick();
	handleInputs();
}

void Game::render()
{
	/*
		- Sterge cadrul precedent
		- Deseneaza cadrul nou
		- Arata cadrul nou in fereastra
	*/

	this->window->clear(sf::Color(20, 24, 45));
	//window->draw(*this->backgroundSprite);

	if (this->currentGameState == gameStates::MainMenu)
	{
		this->test_level.DrawBackground(*window);
		this->mainMenu->draw(*window, this->camera->getCenter(), this->camera->getSize());
	}

	if (this->currentGameState == gameStates::SelectPlayer)
	{
		this->test_level.DrawBackground(*window);
		this->selectPlayerMenu->draw(*window, this->camera->getCenter(), this->camera->getSize());
	}

	if (this->currentGameState == gameStates::Playing)
	{
		this->test_level.DrawLevel(*window);
		this->selectedPlayer->render(*window);
	}

	if (this->currentGameState == gameStates::Won)
	{
		this->test_level.DrawBackground(*window);
		this->winScreen->draw(*window, this->camera->getCenter(), this->camera->getSize());
	}
	window->display();
}

bool Game::checkForClick()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		return true;
	else
		return false;
}

void Game::handleInputs()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		this->currentGameState = gameStates::MainMenu;
}

bool Game::running() const
{
	return this->window->isOpen();
}


