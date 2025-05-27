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
	const sf::Vector2f scalingFactor = { 1.f, 1.f }; //scalarea sprite-urilor

	//Pentru background:
	const int LAYER_WIDTH = 330;
	const int LAYER_HEIGHT = 82;
	const int NUM_LAYERS = 4;
	const int WIN_TILE = 97;

	const float heightSum = 276.0f; 

	std::vector<float> parallaxFactors = { 0.4f, 0.6f, 0.85f, 0.9f, 0.9f };

	std::vector<sf::RectangleShape> backgroundLayers;

					//64 64 120 80
	std::vector<float> layerHeights = {
	80.0f, // Clouds
	120.0f,  // Mountains
	64.0f,  // Trees
	64.0f,    // Rocks
	26.0f,  // Snow
	};

	int** levelMatrix = nullptr; //alocare dinamica
	int** backgroundLayerMatrix = nullptr; //alocare dinamica
	bool hasBackgroundLayer = false;

	int rows;
	int columns;
	int tileSize;

	
	sf::View* view; //alocare dinamica 
	sf::Texture tileset;
	sf::Texture parallaxTexture;
	sf::Sprite *tile;
	sf::Sprite *parallaxSprite;

	sf::RectangleShape backgroundFilter;

	sf::Vector2u spawnPoint;
	sf::Vector2f winPosition;

	void LoadLevelFromFile(const char* filename, const char* tilefile);
public:
	sf::View* InitView(sf::RenderWindow& window);
	Level(const char* filename, const char* tilefile, const char* parallaxFile, bool hasBackground, sf::Vector2u spawnPoint);
	~Level();
	
	void update(float dt, sf::RenderWindow& window,const sf::Vector2f& playerPos);

	void DrawBackground(sf::RenderWindow& window);
	void DrawLevel(sf::RenderWindow &window);
	
	void test_afis_nivel();

	int* operator[](int index);
	sf::Vector2f getTilePosition(int row, int column);
	sf::Vector2f getSpawnPosition();

	int getTileSize();
	int getLevelHeight();
	int getLevelWidth();
	sf::Vector2f getViewCenter() const;
	sf::Vector2f getWinPosition() const;

	sf::Vector2i coordsToLevelPos(const sf::Vector2f& coords);

	bool checkWinCondition(const sf::Vector2f& playerPos) const;
};

