#pragma once

#include "Rope.h"
#include "Level.h"

class Hook : public Rope
{
	const float maxLenght = 300.f;

	const float climbSpeed = 100.f;
	
	float angle = 0.f;
	float angularVelocity = 0.f;

	float springConstant;
	float damping;
	
	sf::Vector2f velocity;
	float speed;
	bool flying;
	bool justAttached;
	
	bool initializedLength;

	float hookRealLength; //Lungimea calculata in cod (in functie de pozitii)

	float hookLength;  //Lungimea dorita 

	sf::RectangleShape hookTip;

	void initLenght();

public:
	Hook();

	float getMaxLength() const;
	sf::Vector2f getDirection() const;
	float getExtraLength() const;
	float getSpringConstant() const;
	float getDamping() const;
	float getHookLength() const;

	bool longerThanMaxLength() const;
	bool hookJustAttached();
	
	void shoot(const sf::Vector2f& from, const sf::Vector2f& to, Level& level, const sf::Vector2f& playerDimensions);
	void detach();
	void shortenHook(float dt);
	void lengthenHook(float dt);

	//void swing(float dt, sf::Vector2f& playerVelocity, const sf::Vector2f& playerPosition, float gravity);

	void draw(sf::RenderTarget& target);

	void updateLength();
	void update(Level& level, const sf::Vector2f& playerPos, float dt, const sf::Vector2f& playerDimensions) override;
};

