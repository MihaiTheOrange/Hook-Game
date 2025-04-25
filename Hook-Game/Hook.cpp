#include "Hook.h"

Hook::Hook() : speed(500.f), flying(false), hookTip({10.f, 2.f}), hookLength(0.f)
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

bool Hook::longerThanMaxLength() const
{
	return this->hookLength > this->maxLenght;
}

void Hook::shoot(const sf::Vector2f& from, const sf::Vector2f& to, Level& level, const sf::Vector2f& playerDimensions)
{
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
	this->hookLength = std::hypot(this->ropeLine[0].position.x - this->ropeLine[1].position.x,
		this->ropeLine[0].position.y - this->ropeLine[1].position.y);
}

void Hook::update(Level &level, const sf::Vector2f& playerPos, float dt, const sf::Vector2f& playerDimensions)
{
	if (this->attached)
	{
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

	/*	this->hookLength = std::hypot(this->ropeLine[0].position.x - this->ropeLine[1].position.x,
			this->ropeLine[0].position.y - this->ropeLine[1].position.y);*/
		this->updateLength();

		//std::cout << this->hookLength << std::endl;
		if (this->hookLength > this->maxLenght)
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
