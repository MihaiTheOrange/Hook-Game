#include "Player.h"


//VA ROG NU VA UITATI IN FISIERUL ASTA
Player::Player() :playerHeight(72.f), playerWidth(36.f), frameIndex(0), animationTimer(0.f), 
movementSpeed(5.f), velocity(0.f, 0.f), onGround(false), isSliding(false), jumpPressed(false)
{
	if (this->playerTexture.loadFromFile("Assets/Player/Textures/idle_sheet.png"))
	{
		std::cout << "Player Image loaded" << std::endl;
		this->playerSprite = new sf::Sprite(playerTexture);
		this->playerSprite->setTextureRect(sf::IntRect({this->frameIndex*48, 0}, {48, 48}));
		this->playerSprite->scale({ 1.5, 1.5 });

		
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
	this->hook = new Hook();

}

Player::~Player()
{
	delete this->playerSprite;
}

void Player::setPlaterPosition(const sf::Vector2f& position)
{
	this->playerSprite->setPosition(position);
}

float Player::getPlayerHeight()
{
	return this->playerHeight;
}

float Player::getPlayerWidth()
{
	return this->playerWidth;
}

sf::Vector2f Player::getPlayerDimensions()
{
	return this->playerHitbox.getSize();
}

void Player::setPosition(const sf::Vector2f& position)
{
	this->playerSprite->setPosition(position);
}

void Player::move(sf::Vector2f &velocity, Level &level, float dt)  //Nu ma intrebati ce se intampla aici 
{
	sf::FloatRect playerBound = this->playerHitbox.getGlobalBounds();
	/*if (onGround)
	{
		velocity.y = 0;
	}*/
	if (velocity.y == 0 || onGround == true)
		isSliding = false;

	if (velocity.x > 0 && isCollidingRight(level, playerBound, velocity.x, dt))
	{
		if (!onGround && velocity.y > 0)	//daca esti langa perete si nu pe pamant activezi sliding
		{
			if (isSliding == true && velocity.y < slidingSpeed)
				velocity.y += this->slidingSpeed * dt;
			else
				if (this->isSliding == false)
				{
					velocity.y += this->slidingSpeed * dt;
					this->isSliding = true;
				}
				else
				{
					velocity.y = this->slidingSpeed;
					this->isSliding = true;
				}
			//std::cout << velocity.y << std::endl;
		}
		velocity.x = 0;
	}
	else
	if (velocity.x < 0 && isCollidingLeft(level, playerBound, velocity.x, dt))
	{
		if (!onGround && velocity.y > 0)	//daca esti langa perete si nu pe pamant activezi sliding
		{
			if (isSliding == true && velocity.y < slidingSpeed)
				velocity.y += this->slidingSpeed * dt;
			else
				if (this->isSliding == false)
				{
					velocity.y += this->slidingSpeed * dt;
					this->isSliding = true;
				}
				else
				{
					velocity.y = this->slidingSpeed;
					this->isSliding = true;
				}
			//std::cout << velocity.y << std::endl;
		}
		velocity.x = 0;
	}
	else
		isSliding = false;


	if (velocity.y < 0 && isCollidingUp(level, playerBound, dt))
	{
		velocity.y = 0;
	}


	this->playerSprite->move({ velocity.x * dt, velocity.y * dt });
}


bool Player::isCollidingRight(Level& level, sf::FloatRect& playerBounds, float velocityX, float dt)
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

bool Player::isCollidingUp(Level& level, sf::FloatRect& playerBounds, float dt)
{
	int leftTile = playerBounds.position.x / level.getTileSize();
	int rightTile = (playerBounds.position.x + playerWidth) / level.getTileSize();
	int topTile = (playerBounds.position.y + velocity.y * dt)/ level.getTileSize();
	for (int i = leftTile; i <= rightTile; i++)
	{
		if (level[topTile][i] != 0)
		{
			//std::cout << "asdasdas"<<topTile<<" "<<i<<" " << level.getTilePosition(topTile, i).x << " " << level.getTilePosition(topTile, i).y << std::endl;
			return true;
		}
	}
	return false;
}

void Player::updateOnGround(Level& level)
{
	if (velocity.y < 0)
	{
		onGround = false;
		return;
	}
	sf::FloatRect playerBound = this->playerHitbox.getGlobalBounds();
	float groundCheckOffset = 5.f;
	int leftTile = playerBound.position.x / level.getTileSize();
	int rightTile = (playerBound.position.x + playerWidth) / level.getTileSize();
	int bottomTile = (playerBound.position.y + playerHeight + groundCheckOffset) / level.getTileSize();
	for (int i = leftTile; i <= rightTile; i++)
	{
		if (level[bottomTile][i] != 0)
		{
			onGround = true;
			velocity.y = 0;
			return;
		}
	}
	onGround = false;
}

void Player::apllyGravity(float dt)
{
	if (!onGround && isSliding == false)
	{
		//std::cout << velocity.y << "gravity" << std::endl;
		velocity.y += gravity * dt;
	}
}



void Player::update(float dt, Level& level, sf::RenderWindow &window)
{
	this->updateOnGround(level);
	this->apllyGravity(dt);
	this->handleInputs(level, dt, window);

	//std::cout << "player " << this->playerHitbox.getPosition().x << " " << this->playerHitbox.getPosition().y << std::endl;
	
	this->hook->update(level, this->playerHitbox.getPosition(), dt, this->getPlayerDimensions());

	this->maxLengthHookCheck();

	//Actualizare animatie
	this->animationTimer += dt;
	if (animationTimer > 0.2f)
	{
		this->playerSprite->setTextureRect(sf::IntRect({ this->frameIndex * 48, 0 }, { 48, 48 }));
		this->frameIndex = (this->frameIndex+1) % 4;
		this->animationTimer = 0.f;
	}

	//Actualizare cooldown wallbounce
	if (this->bounceCooldown > 0.f)
	{
		this->bounceCooldown -= dt;
	}
	if (this->bounceCooldown <= 0.f)
	{
		this->bounceDirrection = 0;
	}
	//Actualizare cooldown air control
	if (this->airControlRestoreCooldown > 0.f)
	{
		this->airControlRestoreCooldown -= dt;
	}
	if (this->airControlRestoreCooldown <= 0.f)
	{
		this->airControlRestoreCooldown = 0.f;
	}

	this->updateBounds();

	
}

void Player::maxLengthHookCheck()
{
	if (this->hook->isAttached())
	{
		if (this->hook->longerThanMaxLength())
		{
			sf::Vector2f direction = this->hook->getDirection();

			if (direction.x >= 0.5f)
			{
				this->velocity.x = speed * 2.f;
			}

		}
	}
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

void Player::handleInputs(Level& level, float dt, sf::RenderWindow& window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && this->bounceDirrection != 1)
	{ 
		if (this->airControlRestoreCooldown <= 0.f)
		{
			velocity.x = -speed;
		}
		else
		{
			velocity.x = std::lerp(velocity.x, -speed, 0.4f);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && this->bounceDirrection != -1)
	{
		if (this->airControlRestoreCooldown <= 0.f)
		{
			velocity.x = speed;
		}
		else
		{
			velocity.x = std::lerp(velocity.x, speed, 0.4f);
		}
	}
	else if(bounceCooldown <= 0)
	{
		if (onGround)
			velocity.x *= friction;
		else
			velocity.x *= airFriction;
	}

	//Saritura
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		if (onGround && jumpPressed == false)
		{
			velocity.y = -jumpStrength;
			jumpPressed = true;
		}
		else if (!onGround && !jumpPressed && this->bounceCooldown <= 0.f)  //Wallbounce
		{
			sf::FloatRect playerBound = this->playerHitbox.getGlobalBounds();
			if (canBounceRight(level, playerBound))
			{
				velocity.x = -jumpStrengthX;
				velocity.y = -jumpStrength;
				this->bounceDirrection = -1;
				this->bounceCooldown = bounceCooldownTime;
				this->airControlRestoreCooldown = airControlRestoreCooldownTime;
				jumpPressed = true;
			}
			else if (canBounceLeft(level, playerBound))
			{
				velocity.x = jumpStrengthX;
				velocity.y = -jumpStrength;
				this->bounceDirrection = 1;
				this->bounceCooldown = bounceCooldownTime;
				this->airControlRestoreCooldown = airControlRestoreCooldownTime;
				jumpPressed = true;
			}
		}

	}
	else
		jumpPressed = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
	{
		this->hook->setAnchor(this->playerHitbox.getPosition());
		this->hook->shoot(this->playerHitbox.getPosition(), sf::Vector2f(1.f, 1.f), level, this->getPlayerDimensions());
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		sf::Vector2i mouseScreenPos = sf::Mouse::getPosition(window);
		sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseScreenPos);
		
		sf::Vector2f playerPos = this->playerHitbox.getPosition();
		
		this->hook->shoot(playerPos, mouseWorldPos, level, this->getPlayerDimensions());
	}

	this->move(velocity, level, dt);

}

bool Player::canBounceLeft(Level& level, sf::FloatRect playerBounds)
{

	int leftTile = (playerBounds.position.x - bounceDistance) / level.getTileSize();
	int topTile = playerBounds.position.y / level.getTileSize();
	int bottomTile = (playerBounds.position.y + playerHeight) / level.getTileSize();
	for (int i = topTile; i <= bottomTile; i++)
	{
		if (level[i][leftTile] != 0)
		{
			return true;
		}
	}
	return false;
}

bool Player::canBounceRight(Level& level, sf::FloatRect playerBounds)
{
	int rightTile = (playerBounds.position.x + playerWidth + bounceDistance) / level.getTileSize();
	int topTile = playerBounds.position.y / level.getTileSize();
	int bottomTile = (playerBounds.position.y + playerHeight) / level.getTileSize();
	for (int i = topTile; i <= bottomTile; i++)
	{
		if (level[i][rightTile] != 0)
		{
			return true;
		}
	}
	return false;
}


void Player::render(sf::RenderTarget& target)
{
	this->drawHitbox(target);
	target.draw(*this->playerSprite);
	this->hook->draw(target);
}
