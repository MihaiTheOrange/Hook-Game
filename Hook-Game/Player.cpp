#include "Player.h"


//VA ROG NU VA UITATI IN FISIERUL ASTA
Player::Player() :playerHeight(96.f), playerWidth(48.f), frameIndex(0), animationTimer(0.f), 
movementSpeed(5.f), velocity(0.f, 0.f), onGround(false)
{
	if (this->playerTexture.loadFromFile("Assets/Player/Textures/idle_sheet.png"))
	{
		std::cout << "Player Image loaded" << std::endl;
		this->playerSprite = new sf::Sprite(playerTexture);
		this->playerSprite->setTextureRect(sf::IntRect({this->frameIndex*48, 0}, {48, 48}));
		this->playerSprite->scale({ 2, 2 });

		
		this->playerHitbox.setSize({ playerWidth, playerHeight });
		this->playerHitbox.setFillColor(sf::Color::Transparent);
		this->playerHitbox.setOutlineColor(sf::Color::Red);
		this->playerHitbox.setOutlineThickness(2.f);

		this->playerHeight = this->playerHeight + heightOffset;
		this->playerWidth = this->playerWidth + widthOffset;


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

void Player::move(sf::Vector2f &velocity, Level &level, float dt)
{
	sf::FloatRect playerBound = this->playerHitbox.getGlobalBounds();
	if (onGround)
	{
		velocity.y = 0;
	}

	if (velocity.x > 0 && isCollidingRinght(level, playerBound, velocity.x, dt))
	{
		velocity.x = 0;
	}

	if (velocity.x < 0 && isCollidingLeft(level, playerBound, velocity.x, dt))
	{
		velocity.x = 0;
	}

	if (velocity.y < 0 && isCollidingUp(level, playerBound))
	{
		velocity.y = 0;
	}

	this->playerSprite->move({velocity.x * dt, 0});
	this->playerSprite->move({0, velocity.y * dt });
}

//bool Player::isColliding(Level& level, sf::FloatRect &playerBounds) //Functie useless
//{
//	int leftTile = playerBounds.position.x / level.getTileSize();
//	int rightTile = (playerBounds.position.x + playerWidth) / level.getTileSize();
//	int topTile = playerBounds.position.y / level.getTileSize();
//	int bottomTile = (playerBounds.position.y + playerHeight) / level.getTileSize();
//	for (int i = topTile; i <= bottomTile; i++)
//	{
//		for (int j = leftTile; j <= rightTile; j++)
//		{
//			//std::cout << i << " " << j << std::endl;
//			if (level[i][j] != 0)
//			{
//				return true;
//			}
//		}
//	}
//	return false;
//}

bool Player::isCollidingRinght(Level& level, sf::FloatRect& playerBounds, float velocityX, float dt)
{
	int rightTile = (playerBounds.position.x + playerWidth + velocityX * dt) / level.getTileSize();
	int topTile = playerBounds.position.y / level.getTileSize();
	int bottomTile = (playerBounds.position.y + playerHeight-10.f) / level.getTileSize();
	for (int i = topTile; i <= bottomTile; i++)
	{
		if (level[i][rightTile] != 0)
		{
			return true;
		}
	}
	return false;
}

bool Player::isCollidingLeft(Level& level, sf::FloatRect& playerBounds, float velocityX, float dt)
{
	int leftTile = (playerBounds.position.x + velocityX * dt)/ level.getTileSize();
	int topTile = playerBounds.position.y / level.getTileSize();
	int bottomTile = (playerBounds.position.y + playerHeight-10.f) / level.getTileSize();
	for (int i = topTile; i <= bottomTile; i++)
	{
		if (level[i][leftTile] != 0)
		{
			return true;
		}
	}
	return false;
}

bool Player::isCollidingUp(Level& level, sf::FloatRect& playerBounds)
{
	int leftTile = playerBounds.position.x / level.getTileSize();
	int rightTile = (playerBounds.position.x + playerWidth) / level.getTileSize();
	int topTile = playerBounds.position.y / level.getTileSize();
	for (int i = leftTile; i <= rightTile; i++)
	{
		if (level[topTile][i] != 0)
		{
			return true;
		}
	}
	return false;
}

void Player::updateOnGround(Level& level)
{
	sf::FloatRect playerBound = this->playerHitbox.getGlobalBounds();
	float groundCheckOffset = 5.f;
	int leftTile = playerBound.position.x / level.getTileSize();
	int rightTile = (playerBound.position.x + playerWidth) / level.getTileSize();
	int bottomTile = (playerBound.position.y + playerHeight) / level.getTileSize();
	for (int i = leftTile; i <= rightTile; i++)
	{
		if (level[bottomTile][i] != 0)
		{
			onGround = true;
			return;
		}
	}
	onGround = false;
}

void Player::apllyGravity(float dt)
{
	if (!onGround)
		velocity.y += gravity * dt;
}



void Player::update(float dt, Level& level)
{
	this->apllyGravity(dt);
	this->handleInputs(level, dt);


	//Actualizare animatie
	this->animationTimer += dt;
	if (animationTimer > 0.2f)
	{
		this->playerSprite->setTextureRect(sf::IntRect({ this->frameIndex * 48, 0 }, { 48, 48 }));
		this->frameIndex = (this->frameIndex+1) % 4;
		this->animationTimer = 0.f;
	}


	this->updateBounds();
	this->updateOnGround(level);
	
}

void Player::updateBounds()
{
	sf::Vector2f spritePos = this->playerSprite->getPosition();

	this->playerHitbox.setSize({ playerWidth, playerHeight});
	this->playerHitbox.setPosition({ spritePos.x + offsetX, spritePos.y + offsetY});
}

void Player::drawHitbox(sf::RenderTarget& target)
{
	target.draw(this->playerHitbox);
}

void Player::handleInputs(Level& level, float dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		velocity.x = -speed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		velocity.x = speed;
	}
	else
	{
		velocity.x *= friction;
	}

	//Saritura
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && onGround)
	{
		velocity.y = -jumpStrength;
		onGround = false;
	}

	this->move(velocity, level, dt);

}


void Player::render(sf::RenderTarget& target)
{
	this->drawHitbox(target);
	target.draw(*this->playerSprite);
}
