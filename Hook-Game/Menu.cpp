#include "Menu.h"

Menu::Menu(sf::Font& font): font(font)
{

}

Menu::~Menu()
{

}

bool Menu::isHovered(sf::FloatRect optionBounds, sf::Vector2f topLeftCoords, sf::RenderWindow& window, size_t i)
{
	optionBounds.position.x += 50.f;
	optionBounds.position.y += 50.f + i * 36.f;
	optionBounds.size.x *= 2.f;
	optionBounds.size.y *= 2.f;
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	mousePosF += topLeftCoords;

	/*std::cout << "Mouse Position: " << mousePosF.x << ", " << mousePosF.y << std::endl;
	std::cout << "Option Bounds: " << optionBounds.size.x << ", " << optionBounds.size.y << ", " 
		<< optionBounds.position.x << ", " << optionBounds.position.y << std::endl;*/

	return optionBounds.contains(mousePosF);
}

void Menu::handleInputs(sf::RenderWindow& window, const sf::Vector2f& viewCenter, const sf::Vector2f& viewSize)
{
	for (rsize_t i = 0 ; i< textOptions.size(); ++i)
	{
		sf::FloatRect optionBounds = textOptions[i].getGlobalBounds();
		if (isHovered(optionBounds, viewCenter - viewSize / 2.f, window, i) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !clicked)
		{
			options[i].action(); 
			break; 
		}
	}
}

void Menu::addOption(std::string text, std::function<void()> action)
{
	MenuOption newOption;
	newOption.label = text;
	newOption.action = action;
	options.push_back(newOption);
}

void Menu::addLabel(std::string text, float size, const sf::Vector2f& position, const sf::Color& color)
{
	sf::Text newLabel(font);
	newLabel.setCharacterSize(size);
	newLabel.setString(text);
	newLabel.setFillColor(color);
	
	LabelOption newLOpt(newLabel);
	newLOpt.relPosition = position;
	textLabels.push_back(newLOpt);
}

void Menu::update(sf::RenderWindow& window, const sf::Vector2f& viewCenter, const sf::Vector2f& viewSize)
{
	textOptions.clear();
	sf::Vector2f topLeft = viewCenter - viewSize / 2.f;
	for (size_t i = 0; i < options.size(); ++i)
	{
		sf::Text optionText(font);
		optionText.setString(options[i].label);
		optionText.setCharacterSize(32);
		optionText.scale({ 0.5f, 0.5f });
		optionText.setPosition({ topLeft.x + 50.f,topLeft.y + 50.f + i * 36.f });
		sf::FloatRect optionBounds = optionText.getGlobalBounds();
		optionText.setFillColor(isHovered(optionBounds, topLeft, window, i) ? sf::Color::Red : sf::Color::White);
		textOptions.push_back(optionText);
	}

	for (auto& textLabel : textLabels)
	{
		textLabel.label.setPosition( {topLeft.x + textLabel.relPosition.x, topLeft.y + textLabel.relPosition.y});
	}

	this->handleInputs(window, viewCenter, viewSize);
}

void Menu::draw(sf::RenderWindow& window, const sf::Vector2f& viewCenter, const sf::Vector2f& viewSize)
{
	for(const auto& textOption : textOptions)
	{
		window.draw(textOption);
	}

	for (const auto& textLabel : textLabels)
	{
		window.draw(textLabel.label);
		std::string str = textLabel.label.getString();
	}
}
