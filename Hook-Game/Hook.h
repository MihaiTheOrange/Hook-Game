#pragma once

#include "Rope.h"
#include "Level.h"

class Hook : public Rope
{
	const float maxLenght = 1000.f;
	
	sf::Vector2f velocity;
	float speed;
	bool flying;

	sf::RectangleShape hookTip;

public:
	Hook();
	
	void shoot(const sf::Vector2f& from, const sf::Vector2f& to, Level& level, const sf::Vector2f& playerDimensions);
	void detach();

	void draw(sf::RenderTarget& target);

	void update(const sf::Vector2f& playerPos, float dt, const sf::Vector2f& playerDimensions) override;
};

