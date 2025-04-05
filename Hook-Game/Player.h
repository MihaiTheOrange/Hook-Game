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
	//Constante
	const float speed = 200.f;   
	const float jumpStrength = 400.f;
	const float jumpStrengthX = 400.f;
	const float gravity = 1000.f;
	const float slidingSpeed = 150.f;
	const float friction = 0.85f;
	
	const float widthOffset = -10.f;
	const float heightOffset = -20.f;

	const float offsetX = 5.f;
	const float offsetY = 10.f;

	const float bounceDistance = 10.f;
	const float bouncingDistance = 10.f;


	 //Variabile
	sf::Texture playerTexture;
	sf::Sprite* playerSprite = nullptr; //alocare dinamica

	sf::RectangleShape playerHitbox;
	
	float playerHeight;
	float playerWidth;

	int frameIndex;
	float animationTimer;

	float bounceCooldown = 0.f;

	sf::Vector2f velocity;

	float movementSpeed;
	bool onGround;
	bool isSliding;
	bool jumpPressed;

	 //Metode private
	void updateBounds();
	void drawHitbox(sf::RenderTarget& target);
	void handleInputs(Level& level, float dt);
	bool canBounceLeft(Level& level, sf::FloatRect playerBounds);
	bool canBounceRight(Level& level, sf::FloatRect playerBounds);
	
public:
	//Constructor / Destructor
	Player();
	~Player();

	float getPlayerHeight();
	void setPosition(const sf::Vector2f& position);
	void move(sf:: Vector2f &velocity , Level& level, float dt);

	/*bool isColliding(Level& level, sf::FloatRect &playerBounds);*/
	bool isCollidingRight(Level& level, sf::FloatRect& playerBounds, float velocityX, float dt);
	bool isCollidingLeft(Level& level, sf::FloatRect& playerBounds, float velocityX, float dt);
	bool isCollidingUp(Level& level, sf::FloatRect& playerBounds, float dt);

	void updateOnGround(Level& level);

	void apllyGravity(float dt);

	void update(float dt, Level& level);

	void render(sf::RenderTarget &target);
};

