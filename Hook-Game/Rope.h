#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <iostream>


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

public:
	
	Rope();
	virtual ~Rope() = default;

	void setAnchor(const sf::Vector2f& anchor);
	bool isAttached();

	virtual void update(const sf::Vector2f& playerPos);
	virtual void draw(sf::RenderTarget& target);
};

