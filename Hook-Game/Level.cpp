#include "Level.h"

Level::Level(const char* filename, const char* tilefile) :tile(nullptr)
{
	LoadLevelFromFile(filename, tilefile);
}

Level::~Level()
{
	for (int i = 0; i < rows; i++)
	{
		delete[] levelMatrix[i];
	}
	delete[] levelMatrix;
	delete tile;
	delete view;

}

void Level::InitView(sf::RenderWindow& window)
{
	// initializam view-ul apoi il setam pentru a incepe in partea de start a nivelului
	float xsize = static_cast<float>(window.getSize().x);
	float ysize = static_cast<float>(window.getSize().y);

	view = new sf::View(sf::FloatRect({ 0.f, 0.f }, { xsize, ysize }));
	
	float mapWidth = columns * tileSize * 2; //*2 pentru ca tile-urile sunt scalate
	float mapHeight = rows * tileSize*2;
	view->setCenter({ xsize / 2 , mapHeight - ysize / 2 }); //x-ul neschimbat y-ul mutat in jos

	window.setView(*view);
}

void Level::LoadLevelFromFile(const char* filename, const char* tilefile)
{
	std::ifstream file;
	file.open(filename);
	if (!file.is_open())
	{
		std::cout << "Eroare la deschiderea fisierului" << std::endl;
	}
	else
	{
		file >> rows >> columns >> tileSize;
		levelMatrix = new int* [rows];
		for (int i = 0; i < rows; i++)
		{
			levelMatrix[i] = new int[columns];
			for (int j = 0; j < columns; j++)
			{
				file >> levelMatrix[i][j];
			}
		}
	}
	file.close();
	if (!tileset.loadFromFile(tilefile))
	{
		std::cout << "Eroare la incarcarea texturii" << std::endl;
	}
	else
	{
		tile = new sf::Sprite(tileset);
	}
}

void Level::DrawLevel(sf::RenderWindow& window)
{
	// parcurgem matricea
	// luam id-ul tile-ului
	// determinam pozitia tile-ului in tileset
	// setam textura pentru sprite-ul tile-ului si il desenam la pozitia corecta 


	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			int tileIndex = levelMatrix[i][j]-1;
			if (tileIndex < 0)
				continue;
			//std::cout << tileIndex << std::endl;
			int tileX = tileIndex % (tileset.getSize().x / tileSize);
			int tileY = tileIndex / (tileset.getSize().x / tileSize);
			
			this->tile->setTextureRect(sf::IntRect({ tileX * tileSize, tileY * tileSize }, { tileSize, tileSize }));
			this->tile->setScale({ 2,2 });

			float posX = (float)(j * tileSize);
			float posY = (float)(i * tileSize)+8;
			//std::cout << j << " " << i << std::endl;
			this->tile->setPosition({posX*2, posY*2});
			window.draw(*tile);
		}
	}

}

void Level::test_afis_nivel()
{
	std::cout << rows << " " << columns << " " << tileSize << std::endl;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			std::cout << levelMatrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

int* Level::operator[](int index)
{
	return levelMatrix[index];
}

int Level::getTileSize()
{
	return this->tileSize*2;
}
