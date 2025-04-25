#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <SFML/Graphics.hpp>
//clasa se ocupa cu incarcarea din fisier txt si desenarea nivelului pe ecran
//campuri necesare: matrice pentru nivel, numar linii si coloane, 
// marimile tile-urilor
//metode necesare: constructor, destructor, incarcare nivel, desenare nivel
class Level
{
private:
	int** levelMatrix; //alocare dinamica
	int rows;
	int columns;
	int tileSize;

	sf::View* view; //alocare dinamica
	sf::Texture tileset;
	sf::Sprite *tile;

	void LoadLevelFromFile(const char* filename, const char* tilefile);
public:
	void InitView(sf::RenderWindow& window);
	Level(const char* filename, const char* tilefile);
	~Level();
	
	void DrawLevel(sf::RenderWindow &window);
	
	void test_afis_nivel();

	int* operator[](int index);
	sf::Vector2f getTilePosition(int row, int column);

	int getTileSize();
	int getLevelHeight();

	sf::Vector2i coordsToLevelPos(const sf::Vector2f& coords);
};

