#include "Hook.h"

Hook::Hook() : speed(400.f), flying(false), hookTip({10.f, 2.f})
{
	this->hookTip.setFillColor(sf::Color(100, 100, 100));
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

void Hook::update(const sf::Vector2f& playerPos, float dt, const sf::Vector2f& playerDimensions)
{
	if (this->attached)
	{
		Rope::update(playerPos, dt, playerDimensions);
		this->hookTip.setPosition(this->anchorPoint);
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
		this->hookTip.setPosition(this->anchorPoint);
	}

}
