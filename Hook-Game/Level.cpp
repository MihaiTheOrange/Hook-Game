#include "Level.h"

Level::Level(const char* filename, const char* tilefile, const char* parallaxFile) : tile(nullptr)
{
	LoadLevelFromFile(filename, tilefile);
	if (!parallaxTexture.loadFromFile(parallaxFile))
	{
		std::cout << "Eroare la incarcarea texturii" << std::endl;
	}
	else
	{
		parallaxTexture.setRepeated(true);

		parallaxSprite = new sf::Sprite(parallaxTexture);
		parallaxSprite->setTextureRect(sf::IntRect({ 0, 0 }, { LAYER_WIDTH, 330 })); // setam dimensiunea sprite-ului

	}
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
	delete parallaxSprite;


}

void Level::InitView(sf::RenderWindow& window)
{
	// initializam view-ul apoi il setam pentru a incepe in partea de start a nivelului
	float xsize = static_cast<float>(window.getSize().x);
	float ysize = static_cast<float>(window.getSize().y);

	view = new sf::View(sf::FloatRect({ 0.f, 0.f }, { xsize, ysize }));
	
	float mapWidth = columns * tileSize ; //*2 pentru ca tile-urile sunt scalate
	float mapHeight = rows * tileSize;
	view->setCenter({ xsize / 2 , mapHeight - ysize / 2 }); //x-ul neschimbat y-ul mutat in jos

	//Filtru de fundal
	this->backgroundFilter.setSize(sf::Vector2f(window.getSize()));
	this->backgroundFilter.setFillColor(sf::Color(20, 40, 40, 50));

	//background parallax
	int posY = 0;
	for (int i = 0; i < NUM_LAYERS; ++i) 
	{
		float layerHeight = layerHeights[i];

		int numOfLayers = this->getLevelWidth()/LAYER_WIDTH;

		sf::RectangleShape layer(sf::Vector2f(static_cast<float>(LAYER_WIDTH-30)* numOfLayers*4, layerHeight));
		layer.setTexture(&parallaxTexture);

		if(i > 0)
			posY += (int)layerHeights[i - 1];
		std::cout << i << " " << posY << std::endl;

		//if (i == NUM_LAYERS-1)
		//	posY -= layerHeights[i];

		layer.setTextureRect(sf::IntRect({ 0, posY }, { LAYER_WIDTH*numOfLayers*4, (int)layerHeight }));

		backgroundLayers.push_back(layer);

		//std::cout << backgroundLayers[i].getSize().x << " " << backgroundLayers[i].getSize().y << std::endl;
	}
	

	view->zoom(0.5f);
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

void Level::update(float dt, sf::RenderWindow& window,const sf::Vector2f& playerPos)
{
	float halfViewWidth = view->getSize().x / 2.0f;
	float halfViewHeight = view->getSize().y / 2.0f;

	float mapWidth = static_cast<float>(getLevelWidth());
	float mapHeight = static_cast<float>(getLevelHeight());

	sf::Vector2f targetCenter = playerPos;

	if (targetCenter.x < halfViewWidth)
		targetCenter.x = halfViewWidth;

	else if (targetCenter.x > mapWidth - halfViewWidth)
		targetCenter.x = mapWidth - halfViewWidth;

	if (targetCenter.y < halfViewHeight)
		targetCenter.y = halfViewHeight;
	else if (targetCenter.y > mapHeight - halfViewHeight)
		targetCenter.y = mapHeight - halfViewHeight;

	view->setCenter(targetCenter);
	

	window.setView(*view);


}

void Level::DrawBackground(sf::RenderWindow& window)
{
	//Background parallax
	sf::Vector2f cameraCenter = view->getCenter();
	sf::Vector2f viewSize = view->getSize();
	float mapHeight = getLevelHeight();


	float curHeight = this->heightSum;

	for (size_t i = 0; i < backgroundLayers.size(); ++i)
	{
		float factor = parallaxFactors[i];
		float layerHeight = layerHeights[i];
		float layerWidth = backgroundLayers[i].getSize().x;

		//float yPosition = mapHeight - curHeight*3/4 - 8*tileSize;

		float cameraButtomY = cameraCenter.y + viewSize.y / 2;
		float yPosition = cameraButtomY - curHeight * 3/4 - 3.5 * tileSize;
		//float yPosition = cameraButtomY - curHeight * 3/4 - 8 * tileSize;

		curHeight -= layerHeight;

		

		//float yPosition = mapHeight - (NUM_LAYERS - i) * layerHeight - 10 * tileSize;

		// Scroll offset based on parallax
		float xOffset = cameraCenter.x * (1 - factor) - layerWidth / 4;

	

		//float startX = xOffset - std::fmod(xOffset, layerWidth) - layerWidth*10;

		// Calculeaza cate tile-uri sunt necesare pentru a umple ecranul
		int tilesNeeded = static_cast<int>(std::ceil(viewSize.x / layerWidth)) + 2;

		for (int j = 0; j < tilesNeeded; ++j)
		{
			sf::RectangleShape& layer = backgroundLayers[i];
			layer.setPosition({ xOffset, yPosition });
			window.draw(layer);
		}
		window.draw(backgroundFilter);
		//view->zoom(2.f);
	}

}

void Level::DrawLevel(sf::RenderWindow& window)
{

	// Desenam background-ul
	// parcurgem matricea
	// luam id-ul tile-ului
	// determinam pozitia tile-ului in tileset
	// setam textura pentru sprite-ul tile-ului si il desenam la pozitia corecta 


	this->DrawBackground(window);

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
			this->tile->setScale(this->scalingFactor);

			float posX = (float)(j * tileSize);
			float posY = (float)(i * tileSize)+8;
			//std::cout << j << " " << i << std::endl;
			this->tile->setPosition({posX, posY});   // scaling scos de pus *2 la ambele reminder
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
	if (index >= rows || index < 0)
		return levelMatrix[0];
	return levelMatrix[index];
}

sf::Vector2f Level::getTilePosition(int row, int column)
{
	return {(float)(row * this->tileSize), (float)(column*this->tileSize+8)};
}

int Level::getTileSize()
{
	return this->tileSize;  //*2 scos de pus reminder
}

int Level::getLevelHeight() //*2 pentru ca tile-urile sunt scalate(SCAPA DE SCALARE)
{
	return this->rows * this->tileSize;  //*2 scos de pus reminder
}

int Level::getLevelWidth()
{
	return this->columns * this->tileSize; //*2 scos de pus reminder
}

sf::Vector2i Level::coordsToLevelPos(const sf::Vector2f& coords)
{
	int x = coords.x / this->getTileSize();
	int y = coords.y / this->getTileSize();
	return { x, y };
}
