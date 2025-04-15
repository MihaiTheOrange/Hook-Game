#include "Rope.h"



Rope::Rope() :ropeLine(sf::PrimitiveType::Lines, 2), attached(false)
{

}

void Rope::setAnchor(const sf::Vector2f& anchor)
{
	this->anchorPoint = anchor;
	this->attached = true;
}

bool Rope::isAttached()
{
	return attached;
}

void Rope::update(const sf::Vector2f& playerPos, float dt)
{
	if (!this->attached)
		return;
	this->startPoint = playerPos;
	this->ropeLine[0].position = this->startPoint;
	this->ropeLine[0].color = sf::Color::Red;

	this->ropeLine[1].position = this->anchorPoint;
	this->ropeLine[1].color = sf::Color::Red;
}

void Rope::draw(sf::RenderTarget& target)
{
	if (this->attached)
		target.draw(this->ropeLine);
}
