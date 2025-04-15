#pragma once

#include "Rope.h"
#include "Level.h"

class Hook : public Rope
{
	sf::Vector2f velocity;
	float speed;
	bool flying;

public:
	Hook();
	
	void shoot(const sf::Vector2f& from, const sf::Vector2f& to, Level& level);
	void detach();

	void draw(sf::RenderTarget& target);

	void update(const sf::Vector2f& playerPos, float dt) override;
};

