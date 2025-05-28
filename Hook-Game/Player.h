#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "Level.h"
#include "Rope.h"
#include "Hook.h"

class Player
{
protected:
	//Constante
	const float speed = 200.f;   
	const float terminalVelocity = 2000.f; 
	const float jumpStrength = 300.f;
	const float jumpStrengthX = 150.f;
	const float gravity = 900.f;
	const float slidingSpeed = 150.f;
	const float friction = 0.8f;
	const float airFriction = 0.95f;
	
	const float widthOffset = -10.f;
	const float heightOffset = -20.f;

	const float offsetX = -4.f;
	const float offsetY = 0.f;

	const float bounceDistance = 10.f;

	const float airControlRestoreCooldownTime = 0.4f;
	const float bounceCooldownTime = 0.2f;


	enum class AnimationStates
	{
		IDLE,
		RUNNING,
		JUMPING,
		BOUNCING,
		CLIMBING
	} currentAnimation;

	bool mirrored = false;

	struct Animation
	{
		sf::Texture texture;
		std::vector<sf::IntRect> frames;
		float animationSpeed = 0.f; // Timpul unui cadru in secunde
	};

	std::unordered_map<AnimationStates, Animation> Animations;

	int numberOfFrames;


	 //Variabile
	sf::Texture playerTexture;
	sf::Sprite* playerSprite = nullptr; //alocare dinamica

	sf::RectangleShape playerHitbox;
	
	Hook* hook = nullptr;
	

	float originalHeight;
	float originalWidth;

	float playerHeight;
	float playerWidth;

	sf::Vector2f scalingFactor; //scalarea sprite-urilor

	int frameIndex;
	float animationTimer;

	float bounceCooldown = 0.f;
	float airControlRestoreCooldown = 0.f;
	int bounceDirrection = 0;  //Pentru blocare input opus directiei bounce-ului

	sf::Vector2f velocity;
	sf::Vector2f desiredMovement;

	float movementSpeed;
	bool onGround;
	bool isSliding;
	bool jumpPressed;
	bool leftPressed;
	bool rightPressed;
	bool isSwinging;
	bool isBouncing;
	bool isClimbing;
	bool isHanging;
	bool frozenAnimation;
	bool calledCout;
	bool clicked;

	 //Metode private
	void updateBounds();
	void drawHitbox(sf::RenderTarget& target);
	void handleInputs(Level& level, float dt, sf::RenderWindow& window);
	bool canBounceLeft(Level& level, sf::FloatRect playerBounds);
	bool canBounceRight(Level& level, sf::FloatRect playerBounds);

	void loadAnimations();
	
public:
	//Constructor / Destructor
	Player();
	~Player();

	void setPlayerAnimation(AnimationStates state);

	void setPlaterPosition(const sf::Vector2f& position);

	float getPlayerHeight();
	float getPlayerWidth();
	
	sf::Vector2f getPlayerDimensions();
	sf::Vector2f getPlayerPosition() const;

	void setPosition(const sf::Vector2f& position);
	void move(sf:: Vector2f &velocity , Level& level, float dt);

	/*bool isColliding(Level& level, sf::FloatRect &playerBounds);*/
	bool isCollidingRight(Level& level, sf::FloatRect& playerBounds, float velocityX, float dt);
	bool isCollidingLeft(Level& level, sf::FloatRect& playerBounds, float velocityX, float dt);
	bool isCollidingUp(Level& level, sf::FloatRect& playerBounds, float dt);

	bool checkWinCondition(Level& level);

	void updateOnGround(Level& level);

	void apllyGravity(float dt);

	void update(float dt, Level& level, sf::RenderWindow& window);
	void handleAnimations(float dt);
	void freezeAnimCheck();

	void maxLengthHookCheck();

	void render(sf::RenderTarget &target);

	void respawn(Level& level);


	friend std::ostream& operator << (std::ostream& c, Player& p);
};

