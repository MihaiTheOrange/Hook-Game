#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "Level.h"

class Player
{
private:
	 //Variabile
	sf::Texture playerTexture;
	sf::Sprite* playerSprite = nullptr; //alocare dinamica
	sf::FloatRect playerBounds;
	sf::RectangleShape playerHitbox;
	float playerHeight;
	float playerWidth;

	int frameIndex;
	float animationTimer;

	float movementSpeed;

	 //Metode private
	void updateBounds();
	void drawHitbox(sf::RenderTarget& target);
public:
	//Constructor / Destructor
	Player();
	~Player();

	float getPlayerHeight();
	void setPosition(const sf::Vector2f& position);
	void move(const float dirX, const float dirY);
	bool isColliding(Level& level);

	void update(float dt, Level& level);

	void render(sf::RenderTarget &target);
};

