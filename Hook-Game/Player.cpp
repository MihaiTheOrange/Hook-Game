#include "Player.h"


//VA ROG NU VA UITATI IN FISIERUL ASTA - UPDATE: Nu mai e cel mai rau fisier :) 
Player::Player() :originalHeight(48.f), originalWidth(24.f), frameIndex(0), animationTimer(0.f), 
movementSpeed(5.f), velocity(0.f, 0.f), desiredMovement(0.f, 0.f), onGround(false), isSliding(false), 
jumpPressed(false), leftPressed(false), rightPressed(false), isSwinging(false), playerHeight(0.f), 
playerWidth(0.f), scalingFactor({ 0.8f, 0.8f }), currentAnimation(AnimationStates::IDLE), numberOfFrames(0),
isBouncing(false), frozenAnimation(false), isClimbing(false), isHanging(false)
{
	if (this->playerTexture.loadFromFile("Assets/Player/Textures/idle_sheet.png"))
	{
		std::cout << "Player Image loaded" << std::endl;
		this->playerSprite = new sf::Sprite(playerTexture);
		this->playerSprite->setTextureRect(sf::IntRect({this->frameIndex*48, 0}, {48, 48}));

		this->playerHeight = this->originalHeight * this->scalingFactor.y;
		this->playerWidth = this->originalWidth * this->scalingFactor.x;


		this->playerSprite->scale(scalingFactor);
		this->playerSprite->setOrigin({ this->playerWidth / 2.f, this->playerHeight / 2.f });

		
		this->playerHitbox.setSize({ playerWidth, playerHeight });
		this->playerHitbox.setFillColor(sf::Color::Transparent);
		this->playerHitbox.setOutlineColor(sf::Color::Red);
		this->playerHitbox.setOutlineThickness(2.f);

		this->playerHeight = this->playerHeight + heightOffset;
		this->playerWidth = this->playerWidth + widthOffset;


		this->loadAnimations();
		this->setPlayerAnimation(AnimationStates::IDLE);
		this->numberOfFrames = this->Animations[this->currentAnimation].frames.size();
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

void Player::setPlayerAnimation(AnimationStates state)
{
	if (this->currentAnimation != state)
	{
		this->currentAnimation = state;
		this->playerSprite->setTextureRect(this->Animations[state].frames[0]);
		this->playerSprite->setTexture(this->Animations[state].texture);
		this->frameIndex = 0;
		this->animationTimer = 0.f;
		//this->playerSprite->setOrigin({ playerWidth / 2.f, playerHeight / 2.f });
		this->playerSprite->setScale(scalingFactor);
		this->numberOfFrames = this->Animations[state].frames.size();
		//std::cout << "number of frames " << this->numberOfFrames << std::endl;

		if(this->mirrored)
			this->playerSprite->setScale({ -scalingFactor.x, scalingFactor.y });
		else
			this->playerSprite->setScale({ scalingFactor.x, scalingFactor.y });

	}
	else
	{
		this->playerSprite->setTextureRect(this->Animations[state].frames[this->frameIndex]);
	}
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

sf::Vector2f Player::getPlayerPosition() const
{
	return this->playerHitbox.getPosition();
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

	/*if (this->isSwinging)
	{
		if (this->hook->longerThanMaxLength())
		{
			std::cout << "hook length I got HERE" << this->hook->getHookLength() << std::endl;
			this->playerSprite->setPosition(this->hook->getAnchorPoint() - this->hook->getDirection() * this->hook->getHookLength());
		}
	}*/

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

bool Player::checkWinCondition(Level& level)
{
	sf::Vector2f winPosition = level.getWinPosition();
	sf::Vector2f playerCenterPosition = this->getPlayerPosition() + sf::Vector2f(playerWidth / 2.f, playerHeight / 2.f);
	sf::Vector2f difference = winPosition - playerCenterPosition;
	float tileSize = static_cast<float>(level.getTileSize());

	//std::cout << playerCenterPosition.x << " " << playerCenterPosition.y << std::endl;
	//std::cout << winPosition.x << " " << winPosition.y << std::endl;
	//std::cout << difference.x << " " << difference.y << std::endl;

	if (difference.x >= -tileSize && difference.x <= tileSize && difference.y >= -tileSize && difference.y <= tileSize)
	{
		return true;
		std::cout << "won";
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
	float groundCheckOffset = 1.f;
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
	if (!onGround && isSliding == false && velocity.y < this->terminalVelocity)
	{
		//std::cout << velocity.y << "gravity" << std::endl;
		velocity.y += gravity * dt;
	}
}



void Player::update(float dt, Level& level, sf::RenderWindow &window)
{
	//std::cout << "direction hook" << this->hook->getDirection().x << " " << this->hook->getDirection().y << " " << isSwinging << std::endl;
	if(!jumpPressed)
		this->isBouncing = false;


	this->updateOnGround(level);
	this->apllyGravity(dt);
	this->handleInputs(level, dt, window);

	this->freezeAnimCheck();
	this->handleAnimations(dt);

	if(checkWinCondition(level))
	{
		std::cout << "You win!" << std::endl;
	}

	if (this->getPlayerPosition().y > level.getLevelHeight())
		this->respawn(level);


	if (leftPressed)
	{
		this->playerSprite->setScale({-scalingFactor.x, scalingFactor.y});
		this->mirrored = true;
	}
	else if (rightPressed)
	{
		this->playerSprite->setScale({ scalingFactor.x, scalingFactor.y });
		this->mirrored = false;
	}

	//std::cout << "player " << this->playerHitbox.getPosition().x << " " << this->playerHitbox.getPosition().y << std::endl;
	
	this->hook->update(level, this->playerHitbox.getPosition(), dt, this->getPlayerDimensions());

	this->maxLengthHookCheck();

	//Actualizare animatie
	this->animationTimer += dt;
	if (animationTimer > Animations[currentAnimation].animationSpeed && !frozenAnimation)
	{
		this->playerSprite->setTextureRect(sf::IntRect({ this->frameIndex * 48, 0 }, { 48, 48 }));
		this->frameIndex = (this->frameIndex+1) % this->numberOfFrames;
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

void Player::handleAnimations(float dt)
{
	//std::cout << this->rightPressed << " " << this->leftPressed << std::endl;
	if (this->isBouncing)
	{
		this->setPlayerAnimation(AnimationStates::BOUNCING);
		return;
	}
	if(this->isClimbing || this->isHanging)
	{
		this->setPlayerAnimation(AnimationStates::CLIMBING);
		return;
	}
	if (this->isSwinging)
	{
		this->setPlayerAnimation(AnimationStates::JUMPING);
		return;
	}
	else if (onGround && (velocity.x == 0.f || this->rightPressed == false && this->leftPressed == false))
	{
		this->setPlayerAnimation(AnimationStates::IDLE);
		return;
	}
	else if (onGround && velocity.x != 0.f)
	{
		this->setPlayerAnimation(AnimationStates::RUNNING);
		return;
	}
	else if (!onGround && velocity.y < 0.f)
	{
		this->setPlayerAnimation(AnimationStates::JUMPING);
		return;
	}
	else if (!onGround && velocity.y > 0.f)
	{
		this->setPlayerAnimation(AnimationStates::JUMPING);
		return;
	}
}

void Player::freezeAnimCheck()
{
	if( this->hook->isAttached() && !this->onGround && !isClimbing && !isSwinging)
	{
		this->frozenAnimation = true;
		this->isHanging = true;
	}
	else
	{
		this->frozenAnimation = false;
		this->isHanging = false;
	}
}

void Player::maxLengthHookCheck()
{
	if (this->hook->isAttached())
	{
		if (this->hook->longerThanMaxLength())
		{
				sf::Vector2f direction = this->hook->getDirection();
				float extraLength = this->hook->getExtraLength();
				float springConstant = this->hook->getSpringConstant();
				float damping = this->hook->getDamping();

				sf::Vector2f springForce = direction * springConstant * extraLength;


				//std::cout << "direction" << direction.x << " " << direction.y << std::endl;

				this->velocity += springForce;

				this->velocity *= damping;
			
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

	//VERIFICARI
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		this->leftPressed = false;
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		this->rightPressed = false;
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		this->desiredMovement = { 0.f, 0.f };

	if (!this->hook->isAttached() || onGround || (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)))
	{
		this->isSwinging = false;
	}


	//Movement normal
	if (this->isSwinging)
	{
		this->hook->swing(dt, desiredMovement, this->playerHitbox.getPosition(), gravity);
	}
	else
	{

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && this->bounceDirrection != 1)
		{
			if (this->airControlRestoreCooldown <= 0.f && this->leftPressed == false)
			{
				if (!hook->isAttached())
				{
					this->isSwinging = false;
					//velocity.x += -speed;
					desiredMovement.x = -speed;
				}
				else
				{
					//std::cout << "swinging" << std::endl;
					if (!onGround)
						this->isSwinging = true;
					//this->hook->swing(dt, velocity, this->playerHitbox.getPosition(), gravity);
				}
			}
			else
			{
				velocity.x = std::lerp(velocity.x, -speed, 0.4f);
				desiredMovement = { 0.f, 0.f };
			}
			this->leftPressed = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && this->bounceDirrection != -1)
		{
			if (this->airControlRestoreCooldown <= 0.f && this->rightPressed == false)
			{
				if (!hook->isAttached())
				{
					this->isSwinging = false;
					//velocity.x += speed;
					desiredMovement.x = speed;
				}
				else
				{
					if (!onGround)
						this->isSwinging = true;
					//std::cout << "swinging" << std::endl;
						//this->hook->swing(dt, velocity, this->playerHitbox.getPosition(), gravity);
						//this->hook->swing(dt, velocity, this->playerHitbox.getPosition(), gravity);
				}
			}
			else
			{
				velocity.x = std::lerp(velocity.x, speed, 0.4f);
				desiredMovement = { 0.f, 0.f };
			}
			this->rightPressed = true;
		}
		else if (bounceCooldown <= 0)
		{
			if (onGround)
				velocity.x *= friction;
			else
				velocity.x *= airFriction;
		}
	}

	
	

	//Climbing
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && this->hook->isAttached())
	{
		hook->shortenHook(dt);
		this->isClimbing = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && this->hook->isAttached())
	{
		hook->lengthenHook(dt);
		this->isClimbing = true;
	}
	else
		this->isClimbing = false;

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
				this->hook->detach();
				velocity.x = -jumpStrengthX;
				velocity.y = -jumpStrength;
				this->bounceDirrection = -1;
				this->bounceCooldown = bounceCooldownTime;
				this->airControlRestoreCooldown = airControlRestoreCooldownTime;
				jumpPressed = true;
				isBouncing = true;
			}
			else if (canBounceLeft(level, playerBound))
			{
				this->hook->detach();
				velocity.x = jumpStrengthX;
				velocity.y = -jumpStrength;
				this->bounceDirrection = 1;
				this->bounceCooldown = bounceCooldownTime;
				this->airControlRestoreCooldown = airControlRestoreCooldownTime;
				jumpPressed = true;
				isBouncing = true;
			}
		}

	}
	else
		jumpPressed = false;


	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		sf::Vector2i mouseScreenPos = sf::Mouse::getPosition(window);
		sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseScreenPos);
		
		sf::Vector2f playerPos = this->playerHitbox.getPosition();
		
		this->hook->shoot(playerPos, mouseWorldPos, level, this->getPlayerDimensions());
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && this->hook->isAttached())
	{
		this->hook->detach();
	}

	this->velocity += this->desiredMovement;
	if (this->desiredMovement.x != 0.f)
		this->velocity.x = 0.7f * this->velocity.x + 0.3f * this->desiredMovement.x;

	this->move(velocity, level, dt);

	//std::cout << "velocity" << this->velocity.x*dt << " " << this->velocity.y*dt << std::endl;
}

bool Player::canBounceLeft(Level& level, sf::FloatRect playerBounds)
{

	int leftTile = (playerBounds.position.x - bounceDistance) / level.getTileSize();
	int topTile = playerBounds.position.y / level.getTileSize();
	int bottomTile = (playerBounds.position.y + playerHeight) / level.getTileSize();

	if (this->isSwinging && this->leftPressed)
	{
		return true;
	}

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

	if (this->isSwinging && this->rightPressed)
	{
		return true;
	}

	for (int i = topTile; i <= bottomTile; i++)
	{
		if (level[i][rightTile] != 0)
		{
			return true;
		}
	}
	return false;
}

void Player::loadAnimations()
{
	Animation idle;
	idle.texture.loadFromFile("Assets/Player/Textures/idle_sheet.png");
	idle.frames.push_back(sf::IntRect({ 0, 0 }, { 48, 48 }));	// setam pozitia fiecarui cadru in textura
	idle.frames.push_back(sf::IntRect({ 1 * 48, 0 }, { 48, 48 }));
	idle.frames.push_back(sf::IntRect({ 2 * 48, 0 }, { 48, 48 }));
	idle.frames.push_back(sf::IntRect({ 3 * 48, 0 }, { 48, 48 }));
	
	idle.animationSpeed = 0.2f; // setam viteza animatiei
	Animations[AnimationStates::IDLE] = idle;

	Animation run;
	run.texture.loadFromFile("Assets/Player/Textures/run_sheet.png");
	run.frames.push_back(sf::IntRect({ 0, 0 }, { 48, 48 }));
	run.frames.push_back(sf::IntRect({ 1 * 48, 0 }, { 48, 48 }));
	run.frames.push_back(sf::IntRect({ 2 * 48, 0 }, { 48, 48 }));
	run.frames.push_back(sf::IntRect({ 3 * 48, 0 }, { 48, 48 }));
	run.frames.push_back(sf::IntRect({ 4 * 48, 0 }, { 48, 48 }));
	run.frames.push_back(sf::IntRect({ 5 * 48, 0 }, { 48, 48 }));

	run.animationSpeed = 0.1f; 
	Animations[AnimationStates::RUNNING] = run;

	Animation jump;
	jump.texture.loadFromFile("Assets/Player/Textures/jump_sheet.png");
	jump.frames.push_back(sf::IntRect({ 0, 0 }, { 48, 48 }));
	jump.frames.push_back(sf::IntRect({ 1 * 48, 0 }, { 48, 48 }));
	jump.frames.push_back(sf::IntRect({ 2 * 48, 0 }, { 48, 48 }));
	jump.frames.push_back(sf::IntRect({ 3 * 48, 0 }, { 48, 48 }));

	jump.animationSpeed = 0.2f;
	Animations[AnimationStates::JUMPING] = jump;

	Animation bounce;
	bounce.texture.loadFromFile("Assets/Player/Textures/doublejump_sheet.png");
	bounce.frames.push_back(sf::IntRect({ 0, 0 }, { 48, 48 }));
	bounce.frames.push_back(sf::IntRect({ 1 * 48, 0 }, { 48, 48 }));
	bounce.frames.push_back(sf::IntRect({ 2 * 48, 0 }, { 48, 48 }));
	bounce.frames.push_back(sf::IntRect({ 3 * 48, 0 }, { 48, 48 }));
	bounce.frames.push_back(sf::IntRect({ 4 * 48, 0 }, { 48, 48 }));
	bounce.frames.push_back(sf::IntRect({ 5 * 48, 0 }, { 48, 48 }));

	bounce.animationSpeed = 0.1f;
	Animations[AnimationStates::BOUNCING] = bounce;

	Animation climb;
	climb.texture.loadFromFile("Assets/Player/Textures/climb_sheet.png");
	climb.frames.push_back(sf::IntRect({ 0, 0 }, { 48, 48 }));
	climb.frames.push_back(sf::IntRect({ 1 * 48, 0 }, { 48, 48 }));
	climb.frames.push_back(sf::IntRect({ 2 * 48, 0 }, { 48, 48 }));
	climb.frames.push_back(sf::IntRect({ 3 * 48, 0 }, { 48, 48 }));
	climb.frames.push_back(sf::IntRect({ 4 * 48, 0 }, { 48, 48 }));
	climb.frames.push_back(sf::IntRect({ 5 * 48, 0 }, { 48, 48 }));

	climb.animationSpeed = 0.2f;
	Animations[AnimationStates::CLIMBING] = climb;

	this->currentAnimation = AnimationStates::IDLE;

}


void Player::render(sf::RenderTarget& target)
{
	//this->drawHitbox(target);
	target.draw(*this->playerSprite);
	this->hook->draw(target);
}

void Player::respawn(Level& level)
{
	this->hook->detach();
	this->setPlaterPosition(level.getSpawnPosition());
}
