#include "Rope.h"

#include "Hook.h"

sf::Vector2f Rope::normalize(const sf::Vector2f& v)
{
	float length = std::sqrt(v.x * v.x + v.y * v.y);
	if (length != 0)
		return v / length;
	else
		return sf::Vector2f({ 0.f, 0.f }); // return zero vector if length is 0

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
//	sf::Vector2f ropeVec = playerPosition - this->anchorPoint;
//	sf::Vector2f ropeDir = normalize(ropeVec);
//	sf::Vector2f tangentDir = { -ropeDir.y, ropeDir.x };
//
//	float torque = gravity * tangentDir.y;
//
//	playerVelocity += tangentDir * torque;
//
//
//
//
//}


// Cod furat 
void Rope::swing(float dt, sf::Vector2f& playerVelocity, const sf::Vector2f& playerPosition, float gravity)
{
	std::cout << "swing" << std::endl;
    sf::Vector2f ropeVec = playerPosition - this->anchorPoint;
    float length = std::sqrt(ropeVec.x * ropeVec.x + ropeVec.y * ropeVec.y);

    if (length == 0.f) return;

    sf::Vector2f ropeDir = ropeVec / length;

    // --- Springy rope correction ---
    float overshoot = length - static_cast<Hook*>(this)->getHookLength();
    sf::Vector2f correction = ropeDir * overshoot * static_cast<Hook*>(this)->getSpringConstant();
    playerVelocity -= correction * dt;

    // --- Damping (reduces bouncing over time) ---
    playerVelocity *= static_cast<Hook*>(this)->getDamping();

    // --- Gravity pull ---
    playerVelocity.y += gravity * dt;

    // --- Tangential (swing) force ---
    
    float buildRate = 8900.f;
    float maxPower = 52200.f;

    // Tangent to the rope direction
    sf::Vector2f tangent(-ropeDir.y, ropeDir.x);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        swingPower += buildRate * dt;
        swingPower = std::min(swingPower, maxPower);
        playerVelocity += tangent * swingPower * dt;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        swingPower += buildRate * dt;
        swingPower = std::min(swingPower, maxPower);
        playerVelocity += -tangent * swingPower * dt;
    }
    else
    {
        swingPower *= 0.9f; // decay momentum when no input
    }

    // --- Gravity boost at bottom of swing ---
    if (std::abs(ropeDir.x) < 0.1f) // nearly vertical rope
    {
        playerVelocity.y += 100.f * dt;
    }

	std::cout << "swingPower: " << swingPower << std::endl;
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
