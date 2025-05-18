#include "Rope.h"


sf::Vector2f Rope::normalize(const sf::Vector2f& v)
{
	float length = std::sqrt(v.x * v.x + v.y * v.y);
	if (length != 0)
		return v / length;
	else
		return sf::Vector2f({ 0.f, 0.f }); // return zero vector if length is 0

}

float Rope::vectorLength(const sf::Vector2f& v)
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f Rope::getAnchorPoint() const
{
	return this->anchorPoint;
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

//void Rope::swing(float dt, sf::Vector2f& playerVelocity, const sf::Vector2f& playerPosition, float gravity)
//{
//	sf::Vector2f ropeVec = this->startPoint - this->anchorPoint;
//	sf::Vector2f ropeDir = normalize(ropeVec);
//	sf::Vector2f tangentDir = { -ropeDir.y, ropeDir.x };
//
//
//	float torque = 4.f;
//	float maxPower = 100.f;
//
//	if (this->swingPower > maxPower)
//		this->swingPower = maxPower;
//	else
//		this->swingPower += torque;
//
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
//		playerVelocity += tangentDir * this->swingPower;
//	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
//		playerVelocity += -tangentDir * this->swingPower;
//	
//	std::cout << "player vel" << playerVelocity.x << " " << playerVelocity.y << std::endl;
//	std::cout << "tangent" << tangentDir.x << " " << tangentDir.y << std::endl;
//
//}


// Cod furat 
void Rope::swing(float dt, sf::Vector2f& playerVelocity, const sf::Vector2f& playerPosition, float gravity)
{
	sf::Vector2f ropeVec = playerPosition - this->anchorPoint;
	sf::Vector2f ropeDir = normalize(ropeVec);
	sf::Vector2f tangentDir = { -ropeDir.y, ropeDir.x }; // perpendicular to rope

	float swingForce = 20.f; // some fixed value you can tweak

	float tensionFactor = 1.f - std::abs(ropeDir.x); 
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		// Add force along the tangent (positive direction)
		playerVelocity += tangentDir * swingForce;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		// Add force in opposite direction
		playerVelocity -= tangentDir * swingForce;
	}

	// Optionally clamp velocity if needed to avoid huge spikes
	float maxSpeed = 180.f;
	if (vectorLength(playerVelocity) > maxSpeed)
		playerVelocity = normalize(playerVelocity) * maxSpeed;
	playerVelocity.y /= 3.f;
	
	playerVelocity.x *= 0.9f; // Apply some damping to the x velocity

	/*if ((ropeDir.x < 0.f && ropeDir.y < -0.1f) || (ropeDir.x > 0.f && ropeDir.y < 0.f))
		playerVelocity = { 0,0 };*/
}


void Rope::update(Level& level, const sf::Vector2f& playerPos, float dt, const sf::Vector2f& playerDimensions)
{
	if (!this->attached)
		return;
	this->startPoint = playerPos;
	this->startPoint.x += playerDimensions.x;
	this->startPoint.y += playerDimensions.y / 2.f;

	this->ropeLine[0].position = this->startPoint;
	//this->ropeLine[0].color = sf::Color::Red;

	this->ropeLine[1].position = this->anchorPoint;
	//this->ropeLine[1].color = sf::Color::Red;
}

void Rope::draw(sf::RenderTarget& target)
{
	if (this->attached)
		target.draw(this->ropeLine);
}
