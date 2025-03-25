#include "Player.h"

Player::Player():playerHeight(75.f), playerWidth(48.f), frameIndex(0), animationTimer(0.f), movementSpeed(5.f)
{
	if (this->playerTexture.loadFromFile("Assets/Player/Textures/idle_sheet.png"))
	{
		std::cout << "Player Image loaded" << std::endl;
		this->playerSprite = new sf::Sprite(playerTexture);
		this->playerSprite->setTextureRect(sf::IntRect({this->frameIndex*48, 0}, {48, 48}));
		this->playerSprite->scale({ 2, 2 });

		this->playerBounds = this->playerSprite->getGlobalBounds();
		this->playerBounds.size.x = playerWidth;
		this->playerBounds.size.y = playerHeight;
		
		this->playerHitbox.setSize({ playerWidth, playerHeight });
		this->playerHitbox.setFillColor(sf::Color::Transparent);
		this->playerHitbox.setOutlineColor(sf::Color::Red);
		this->playerHitbox.setOutlineThickness(2.f);


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

bool Player::isColliding(Level& level)
{
	int leftTile = playerBounds.position.x / level.getTileSize();
	int rightTile = (playerBounds.position.x + playerWidth) / level.getTileSize();
	int topTile = playerBounds.position.y / level.getTileSize();
	int bottomTile = (playerBounds.position.y + playerHeight) / level.getTileSize();
	for (int i = topTile; i <= bottomTile; i++)
	{
		for (int j = leftTile; j <= rightTile; j++)
		{
			if (level[i][j] != 0)
			{
				std::cout << "Collision detected" << std::endl;
				return true;
			}
		}
	}
	return false;
}

void Player::update(float dt, Level& level)
{
	this->animationTimer += dt;
	if (animationTimer > 0.2f)
	{
		this->playerSprite->setTextureRect(sf::IntRect({ this->frameIndex * 48, 0 }, { 48, 48 }));
		this->frameIndex = (this->frameIndex+1) % 4;
		this->animationTimer = 0.f;
	}
	this->updateBounds();
	if (!isColliding(level))
	{
		std::cout<<"not coliding boss" << std::endl;
	}
}

void Player::updateBounds()
{
	this->playerBounds = this->playerSprite->getGlobalBounds();
	this->playerBounds.size.x = playerWidth;
	this->playerBounds.size.y = playerHeight;
	this->playerBounds.size.y += 20;
	this->playerBounds.position.x += 5;

	this->playerHitbox.setPosition(this->playerBounds.position);
	this->playerHitbox.setSize(this->playerBounds.size);
}

void Player::drawHitbox(sf::RenderTarget& target)
{
	target.draw(this->playerHitbox);
}

void Player::render(sf::RenderTarget& target)
{
	this->drawHitbox(target);
	target.draw(*this->playerSprite);
}
