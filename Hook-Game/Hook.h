#pragma once

#include "Rope.h"
#include "Level.h"

class Hook : public Rope
{
	const float maxLenght = 500.f;
	
	sf::Vector2f velocity;
	float speed;
	bool flying;
	float hookLength;

	sf::RectangleShape hookTip;

public:
	Hook();

	float getMaxLength() const;
	sf::Vector2f getDirection() const;
	bool longerThanMaxLength() const;
	
	void shoot(const sf::Vector2f& from, const sf::Vector2f& to, Level& level, const sf::Vector2f& playerDimensions);
	void detach();

	void draw(sf::RenderTarget& target);

	void updateLength();
	void update(Level& level, const sf::Vector2f& playerPos, float dt, const sf::Vector2f& playerDimensions) override;
};

