#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class Menu
{
	sf::Font font;

	struct MenuOption
	{
		std::string label;
		std::function<void()> action;
	};

	struct LabelOption
	{
		sf::Text label;
		sf::Vector2f relPosition;
	};

	std::vector<MenuOption> options;
	std::vector<sf::Text> textOptions;

	std::vector<LabelOption> textLabels;

public:
	Menu(sf::Font& font);
	~Menu();

	bool isHovered(sf::FloatRect optionBounds, sf::Vector2f topLeftCoords, sf::RenderWindow& window, size_t i);
	void handleInputs(sf::RenderWindow& window, const sf::Vector2f& viewCenter, const sf::Vector2f& viewSize);

	void addOption(std::string text, std::function<void()> action);
	void addLabel(std::string text, float size, const sf::Vector2f& position, const sf::Color& color);

	void update(sf::RenderWindow& window, const sf::Vector2f& viewCenter, const sf::Vector2f& viewSize);
	void draw(sf::RenderWindow& window, const sf::Vector2f& viewCenter, const sf::Vector2f& viewSize);
};

