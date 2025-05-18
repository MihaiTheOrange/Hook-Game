#include "Hook.h"

void Hook::initLenght()
{
	if (!this->initializedLength)
	{
		this->hookLength = this->hookRealLength;
		this->initializedLength = true;	
		//std::cout << "hook length " << this->hookLength << std::endl;
	}
}

Hook::Hook() : speed(400.f), flying(false), hookTip({10.f, 2.f}), hookRealLength(0.f),
springConstant(2.5f), damping(0.8f), initializedLength(false), hookLength(0.f)
{
	this->hookTip.setFillColor(sf::Color(100, 100, 100));
}

float Hook::getMaxLength() const
{
	return this->maxLenght;
}

sf::Vector2f Hook::getDirection() const
{
	sf::Vector2f direction = this->ropeLine[1].position - this->ropeLine[0].position;
	return normalize(direction);
}

float Hook::getExtraLength() const
{
	if (this->hookRealLength > this->hookLength)
		return this->hookRealLength - this->hookLength;
	else
		return this->hookRealLength - this->maxLenght;
}

float Hook::getSpringConstant() const
{
	return this->springConstant;
}

float Hook::getDamping() const
{
	return this->damping;
}

float Hook::getHookLength() const
{
	return this->hookLength;
}

bool Hook::longerThanMaxLength() const
{
	//std::cout << "hook real length " << this->hookRealLength << std::endl;
	//std::cout << "hook length " << this->hookLength << std::endl;
	return (this->hookRealLength > this->maxLenght || this->hookRealLength > this->hookLength);
}

void Hook::shoot(const sf::Vector2f& from, const sf::Vector2f& to, Level& level, const sf::Vector2f& playerDimensions)
{
	this->initializedLength = false;

	this->startPoint = from;
	this->startPoint.x += playerDimensions.x;
	this->startPoint.y += playerDimensions.y / 2.f; 


	sf::Vector2f direction = normalize(to - from); 
	
	this->velocity = direction * speed;

	this->anchorPoint = from + direction * 10.f; 

	this->attached = false;
	this->flying = true;
}

void Hook::detach()
{
	this->attached = false;
	this->flying = false;
}

void Hook::shortenHook(float dt)
{
	if (this->hookLength - this->climbSpeed * dt > 0.f)
		this->hookLength -= this->climbSpeed * dt;
}

void Hook::lengthenHook(float dt)
{
	if (this->hookLength + this->climbSpeed * dt < this->maxLenght)
		this->hookLength += this->climbSpeed * dt;
}

//void Hook::swing(float dt, sf::Vector2f& playerVelocity, const sf::Vector2f& playerPosition, float gravity)
//{
//	sf::Vector2f ropeVec = startPoint - anchorPoint;
//	float currentLength = vectorLength(ropeVec);
//	sf::Vector2f dir = normalize(ropeVec);
//
//	float angle = std::atan2(dir.x, dir.y);
//	this->angle = angle;
//
//	float g = gravity;
//	float angularAcc = -(g / this->hookLength) * std::sin(this->angle);
//
//	this->angularVelocity += angularAcc * dt;
//
//	this->angularVelocity *= this->damping;
//
//	this->angle += this->angularVelocity * dt;
//
//	std::cout << "angle " << this->angle << std::endl;
//	std::cout << "angular velocity " << this->angularVelocity << std::endl;
//	
//	sf::Vector2f offset = {
//	this->hookLength * std::sin(this->angle),
//	this->hookLength * std::cos(this->angle)
//	};
//
//	//sf::Vector2f newPlayerPos = anchorPoint + offset;
//
//	
//
//	float inputTorque = 0.f;
//	float torqueStrength = 50.f; // tweak as needed
//
//	if (isKeyPressed(sf::Keyboard::Key::A)) 
//	{
//		inputTorque = -torqueStrength;
//	}
//	else if (isKeyPressed(sf::Keyboard::Key::D)) 
//	{
//		inputTorque = torqueStrength;
//	}
//
//	this->angularVelocity += inputTorque;
//
//	float maxAngularVelocity = 10.f;
//	if (angularVelocity > maxAngularVelocity) angularVelocity = maxAngularVelocity;
//	if (angularVelocity < -maxAngularVelocity) angularVelocity = -maxAngularVelocity;
//
//	float speed = angularVelocity * this->hookLength;
//	sf::Vector2f tangent = { std::cos(this->angle), -std::sin(this->angle) };
//	sf::Vector2f newVelocity = tangent * speed;
//
//	playerVelocity = newVelocity;
//
//}


//void Hook::swing(float dt, sf::Vector2f& playerVelocity, const sf::Vector2f& playerPosition, float gravity)
//{
//	// === Parameters ===
//	float torqueStrength = 50.f;
//	float damping = 0.99f;
//	float maxAngularVelocity = 10.f;
//	this->angle = 0.1f;
//	// === Gravity-based angular acceleration ===
//	float angularAcc = -(gravity / this->hookLength) * std::sin(this->angle);
//	this->angularVelocity += angularAcc * dt;
//
//	// === Optional: Add torque from player input ===
//	if (std::abs(this->angle) > 0.1f) {
//		if (isKeyPressed(sf::Keyboard::Key::A))
//			this->angularVelocity -= torqueStrength * dt;
//		if (isKeyPressed(sf::Keyboard::Key::D))
//			this->angularVelocity += torqueStrength * dt;
//	}
//
//	// === Apply damping and clamp ===
//	this->angularVelocity *= damping;
//
//	if (this->angularVelocity > maxAngularVelocity)
//		this->angularVelocity = maxAngularVelocity;
//	else if (this->angularVelocity < -maxAngularVelocity)
//		this->angularVelocity = -maxAngularVelocity;
//
//	// === Update angle based on angular velocity ===
//	this->angle += this->angularVelocity * dt;
//
//	// === Calculate position from angle (optional, useful for rope render) ===
//	sf::Vector2f offset = {
//		hookLength * std::sin(this->angle),
//		hookLength * std::cos(this->angle)
//	};
//	sf::Vector2f newPlayerPosition = anchorPoint + offset;
//
//	// === Update player velocity (tangent direction) ===
//	float tangentialSpeed = this->angularVelocity * hookLength;
//	sf::Vector2f tangent = { std::cos(this->angle), -std::sin(this->angle) };
//	playerVelocity = tangent * tangentialSpeed;
//
//	// Debug
//	std::cout << "player velocity " << playerVelocity.x << " " << playerVelocity.y << std::endl;
//}



void Hook::draw(sf::RenderTarget& target)
{
	if (this->attached)
	{ 
		Rope::draw(target);
		return;
	}
	if (this->flying)
	{
		target.draw(this->ropeLine);
		target.draw(this->hookTip);
	}
}

void Hook::updateLength()
{
	this->hookRealLength = std::hypot(this->ropeLine[0].position.x - this->ropeLine[1].position.x,
		this->ropeLine[0].position.y - this->ropeLine[1].position.y);
}

void Hook::update(Level &level, const sf::Vector2f& playerPos, float dt, const sf::Vector2f& playerDimensions)
{
	if (this->attached)
	{
		this->initLenght();
		Rope::update(level, playerPos, dt, playerDimensions);
		this->hookTip.setPosition(this->anchorPoint);
		this->updateLength();
		return;
	}

	if (this->flying)
	{
		//std::cout << ropeLine[0].position.x << std::endl;
		this->anchorPoint += this->velocity * dt; // De schimbat cu dt

		this->ropeLine[0].position = playerPos;
		this->ropeLine[0].position.x += playerDimensions.x;
		this->ropeLine[0].position.y += playerDimensions.y / 2.f;

		this->ropeLine[1].position = this->anchorPoint;

	/*	this->hookRealLength = std::hypot(this->ropeLine[0].position.x - this->ropeLine[1].position.x,
			this->ropeLine[0].position.y - this->ropeLine[1].position.y);*/
		this->updateLength();

		//std::cout << this->hookRealLength << std::endl;
		if (this->hookRealLength > this->maxLenght)
		{
			this->flying = false;
			this->attached = false;
			this->ropeLine[1].position = this->ropeLine[0].position;
		}
		/*std::cout << "hook " << this->hookTip.getPosition().x << " " << 
			this->hookTip.getPosition().y << std::endl;*/

		this->hookTip.setPosition(this->anchorPoint);

		sf::Vector2i tipPos = level.coordsToLevelPos(this->anchorPoint);

		//std::cout << "tippos" << tipPos.x << " " << tipPos.y << std::endl;
		//std::cout << "calculat" << this->hookTip.getPosition().x / level.getTileSize() <<
			//" " << this->hookTip.getPosition().y / level.getTileSize() << std::endl;

		if (level[tipPos.y][tipPos.x] != 0)
		{
			this->attached = true;
			this->flying = false;
			//this->ropeLine[1].position = level.getTilePosition(tipPos.x, tipPos.y);
			this->hookTip.setPosition(level.getTilePosition(tipPos.x, tipPos.y));


			//std::cout << level[tipPos.x][tipPos.y] << std::endl << tipPos.x << " " << tipPos.y << std::endl;


		}
		
		//std::cout << tipPos.x << " " << tipPos.y << std::endl;
	}

}
