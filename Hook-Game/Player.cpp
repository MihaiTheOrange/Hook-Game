#include "Player.h"

Player::Player():playerHeight(96.f), frameIndex(0), animationTimer(0.f), movementSpeed(5.f)
{
	if (this->playerTexture.loadFromFile("Assets/Player/Textures/idle_sheet.png"))
	{
		std::cout << "Player Image loaded" << std::endl;
		this->playerSprite = new sf::Sprite(playerTexture);
		this->playerSprite->setTextureRect(sf::IntRect({this->frameIndex*48, 0}, {48, 48}));
		this->playerSprite->scale({ 2,2 });
		this->frameIndex++;
	}
	else
	{
		std::cout << "Player image load failed!" << std::endl;
	}
}

Player::~Player()
{
	delete this->playerSprite;
}

float Player::getPlayerHeight()
{
	return this->playerHeight;
}

void Player::setPosition(const sf::Vector2f& position)
{
	this->playerSprite->setPosition(position);
}

void Player::move(const float dirX, const float dirY)
{
	this->playerSprite->move({this->movementSpeed*dirX, this->movementSpeed*dirY});
}

void Player::update(float dt)
{
	this->animationTimer += dt;
	if (animationTimer > 0.2f)
	{
		this->playerSprite->setTextureRect(sf::IntRect({ this->frameIndex * 48, 0 }, { 48, 48 }));
		this->frameIndex = (this->frameIndex+1) % 4;
		this->animationTimer = 0.f;
	}
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(*this->playerSprite);
}
