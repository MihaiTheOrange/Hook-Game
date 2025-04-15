#include "Hook.h"

Hook::Hook() : speed(400.f), flying(false)
{

}

void Hook::shoot(const sf::Vector2f& from, const sf::Vector2f& to, Level& level)
{
	this->startPoint = from;
	this->velocity = to * speed;
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
	}
}

void Hook::update(const sf::Vector2f& playerPos, float dt)
{
	if (this->attached)
	{
		Rope::update(playerPos, dt);
		return;
	}

	if (this->flying)
	{
		//std::cout << ropeLine[0].position.x << std::endl;
		this->anchorPoint += this->velocity * dt; // De schimbat cu dt
		this->ropeLine[0].position = playerPos;

		this->ropeLine[1].position = this->anchorPoint;
	}

}
