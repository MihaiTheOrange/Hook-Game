#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <iostream>

#include "Level.h"


//Clasa responsabila pentru tot ce tine de carligul de catarare

//Metode: update, draw, setAnchor, isAttached
//Draw va folosi sf::Vertex(Line)
//Clasa va fi mostenita de Hook
class Rope
{
protected:
	sf::Vector2f anchorPoint; //pozitia sfarsitului carligului
	sf::Vector2f startPoint; //pozitia inceputului carligului

	sf::VertexArray ropeLine;
	bool attached;  //daca atinge un tile suportat va fi true
	
	float swingPower = 0.f;
public:
	static sf::Vector2f normalize(const sf::Vector2f& v);

	sf::Vector2f getAnchorPoint() const;

	Rope();
	virtual ~Rope() = default;

	void setAnchor(const sf::Vector2f& anchor);
	bool isAttached();

	void swing(float dt, sf::Vector2f& playerVelocity, const sf::Vector2f& playerPosition, float gravity);

	virtual void update(Level& level, const sf::Vector2f& playerPos, float dt, const sf::Vector2f& playerDimensions);
	virtual void draw(sf::RenderTarget& target);
};

