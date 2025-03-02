#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class Player
{
private:
	 //Variabile
	sf::Texture playerTexture;
	sf::Sprite *playerSprite=nullptr;
	float playerHeight;

	int frameIndex;
	float animationTimer;

	float movementSpeed;
	 //Metode private
public:
	//Constructor / Destructor
	Player();
	~Player();

	float getPlayerHeight();
	void setPosition(const sf::Vector2f& position);
	void move(const float dirX, const float dirY);

	void update(float dt);
	void render(sf::RenderTarget &target);
};

