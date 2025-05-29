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
springConstant(2.5f), damping(0.8f), initializedLength(false), hookLength(0.f), justAttached(false)
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

bool Hook::hookJustAttached()
{
	if (justAttached)
		return true;
	return false;
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
	justAttached = false;
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
			if (justAttached == false)
				justAttached = true;
			this->attached = true;
			this->flying = false;
			//this->ropeLine[1].position = level.getTilePosition(tipPos.x, tipPos.y);
			this->hookTip.setPosition(level.getTilePosition(tipPos.x, tipPos.y));


			//std::cout << level[tipPos.x][tipPos.y] << std::endl << tipPos.x << " " << tipPos.y << std::endl;


		}
		
		//std::cout << tipPos.x << " " << tipPos.y << std::endl;
	}

}
