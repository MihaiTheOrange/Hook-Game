#include "Game.h"


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


// Constructor / Desctructor
Game::Game() : test_level("Assets/Levels/Test_level/test.txt", "Assets/Levels/Test_level/Snow.png")
{
	this->initVariables();
	this->initWindow();
	this->test_level.InitView(*window);
	//this->initBackground();
	this->player.setPosition({0.f, window->getSize().y - player.getPlayerHeight()});
}

Game::~Game()
{
	delete this->window;
	delete this->backgroundSprite;
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		this->player.move(-1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		this->player.move(1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		this->player.move(0.f, -1.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		this->player.move(0.f, 1.f);
	}
	
}

void Game::update(float dt)
{
	this->pollEvents();
	player.update(dt, test_level);
}

void Game::render()
{
	/*
		- Sterge cadrul precedent
		- Deseneaza cadrul nou
		- Arata cadrul nou in fereastra
	*/

	window->clear();
	//window->draw(*this->backgroundSprite);
	this->test_level.DrawLevel(*window);
	this->player.render(*window);
	window->display();
}

bool Game::running() const
{
	return this->window->isOpen();
}


