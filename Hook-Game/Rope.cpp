#include "Rope.h"


sf::Vector2f Rope::normalize(const sf::Vector2f& v)
{
	float length = std::sqrt(v.x * v.x + v.y * v.y);
	if (length != 0)
		return v / length;
	else
		return sf::Vector2f({ 0.f, 0.f }); // return zero vector if length is 0

}

Rope::Rope() :ropeLine(sf::PrimitiveType::Lines, 2), attached(false)
{
	ropeLine[0].color = sf::Color(139, 69, 19);
	ropeLine[1].color = sf::Color(139, 69, 19);
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

void Rope::update(const sf::Vector2f& playerPos, float dt, const sf::Vector2f& playerDimensions)
{
	if (!this->attached)
		return;
	this->startPoint = playerPos;
	this->startPoint.x += playerDimensions.x;
	this->startPoint.y += playerDimensions.y / 2.f;

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
