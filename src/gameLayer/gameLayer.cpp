#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include <GLFW/glfw3.h>
#include "definitions.h"
#include <chrono>
#include <thread>

class HelicopterSize {
public :
	int sizeX = 90;
	int sizeY = 30;
};

class AntiaircraftSize {
public:
	int sizeX = 90;
	int sizeY = 30;
};

class BuildingSize {
public:
	int sizeX = (platform::getFrameBufferSizeX() / 20) * 2;
	int sizeY = (platform::getFrameBufferSizeY() / 20) * 13;
};


struct GameData
{
	// Posição dos objetos na tela
	glm::vec2 rectPos = {0,0};
	glm::vec2 buildingA = { 0, 0 };
	glm::vec2 buildingB = { 0, 0 };
	glm::vec2 antiaircraftAPos = { 100, 100 };
	glm::vec2 antiaircraftBPos = { 100, 100 };
	glm::vec2 bridge = { 0, 0 };
	glm::vec2 ground = { 0, 0 };

}gameData;

//gl2d::Renderer2D renderer;
//gl2d::Texture t;
//gl2d::Texture at;
//gl2d::Font font;

int points = 0;
int option = 0;
bool hasRun = true;

bool initGame()
{
	//initializing stuff for the renderer
	gl2d::init();
	definitions::renderer.create();

	//loading the saved data. Loading an entire structure like this makes savind game data very easy.
	platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

	definitions::building.loadFromFile(RESOURCES_PATH "building.png", false);
	definitions::bridge.loadFromFile(RESOURCES_PATH "bridge.png", false);
	definitions::ground.loadFromFile(RESOURCES_PATH "ground.png", false);
	definitions::t.loadFromFile(RESOURCES_PATH "test.png", false);
	definitions::font.createFromFile(RESOURCES_PATH "roboto_black.ttf");

	// antiaircraft assets
	definitions::antiair.loadFromFile(RESOURCES_PATH "antiaircraft.png", false);

	return true;
}



bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX(); //window w
	h = platform::getFrameBufferSizeY(); //window h
	
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT); //clear screen

	definitions::renderer.updateWindowMetrics(w, h);

#pragma endregion

	HelicopterSize helicopterSizes = HelicopterSize();

	AntiaircraftSize antiaircraftSizes = AntiaircraftSize();

	BuildingSize buildingSize = BuildingSize();

	// Declara as strings de posição (x e y) e converte-as do objeto original que era em outro formato (possivelmente float ou double)
	std::string playerPosx = std::to_string(gameData.rectPos.x);
	std::string playerPosy = std::to_string(gameData.rectPos.y);

	//if (int(gameData.rectPos.x) == 0 || int(gameData.rectPos.x) == (platform::getFrameBufferSizeX()) - 100) {
	//	helicopterSizes.sizeX= 0;
	//}
	bool evalX = ((int(gameData.rectPos.x) >= 320) && (int(gameData.rectPos.x) < 330));
	bool evalY = ((int(gameData.rectPos.y) >= 365) && (int(gameData.rectPos.y) < 380));


	if (int(gameData.rectPos.y) == 0 || (int(gameData.rectPos.y) == (platform::getFrameBufferSizeY() - 40)) || (evalX && evalY)) {
		std::exit(0);
	}
	
	if (int(gameData.rectPos.x) == 350) {
		points += 10;
	}

	std::string objPosX = std::to_string(gameData.buildingB.x);
	std::string objPosY = std::to_string(gameData.buildingB.y);

	if (platform::isKeyHeld(platform::Button::Left))
	{
		// Define a velocidade do objeto para esquerda
		gameData.rectPos.x -= deltaTime * 200;
	}
	if (platform::isKeyHeld(platform::Button::Right))
	{
		// Define a velocidade do objeto para direita
		gameData.rectPos.x += deltaTime * 200;
	}
	if (platform::isKeyHeld(platform::Button::Up))
	{
		// Define a velocidade do objeto para cima
		gameData.rectPos.y -= deltaTime * 200;
	}
	if (platform::isKeyHeld(platform::Button::Down))
	{
		// Define a velocidade do objeto para baixo
		gameData.rectPos.y += deltaTime * 200;
		// Imprime no console
		//std::cout << "x: " + playerPosx + " ";
		//std::cout << "y: " + playerPosy + "\n";
	}

	// Verifica se o mouse esquerdo foi pressionado
	definitions::setMousePosition();

	if (platform::isLMousePressed()) {
		definitions::setMousePosition();
		bool posX = definitions::mousePos.mouseX >= definitions::menuItemsPos.easyCornerXLeft && definitions::mousePos.mouseX < definitions::menuItemsPos.easyCornerXRight;
		bool posY = definitions::mousePos.mouseY >= definitions::menuItemsPos.easyCornerYLeft && definitions::mousePos.mouseY < definitions::menuItemsPos.easyCornerYRight;
		bool posYNormal = definitions::mousePos.mouseY >= definitions::menuItemsPos.easyCornerYLeft + 30 && definitions::mousePos.mouseY < definitions::menuItemsPos.easyCornerYRight + 30;
		bool posYDificil = definitions::mousePos.mouseY >= definitions::menuItemsPos.easyCornerYLeft + 60 && definitions::mousePos.mouseY < definitions::menuItemsPos.easyCornerYRight + 60;
		if (posX && posY) {
			std::cout << "Pressionou Facil \n";
			option = 1;
		}
		else if (posX && posYNormal) {
			std::cout << "Pressionou Normal \n";
			option = 2;
		}
		else if (posX && posYDificil) {
			std::cout << "Pressionou Dificil \n";
			option = 3;
		}
	}

	gameData.buildingA = glm::clamp(gameData.buildingA, glm::vec2{ 0, h - buildingSize.sizeY }, glm::vec2{ 0, h - buildingSize.sizeY });
	gameData.buildingB = glm::clamp(gameData.buildingB, glm::vec2{ w - buildingSize.sizeX, h - buildingSize.sizeY }, glm::vec2{ w - buildingSize.sizeX, h - buildingSize.sizeY });
	gameData.ground = glm::clamp(gameData.ground, glm::vec2{ 0, (h / 12) * 11 }, glm::vec2{ w,h });
	gameData.bridge = glm::clamp(gameData.bridge, glm::vec2{ (w / 10) * 2, (h / 12) * 10.8 }, glm::vec2{ w,h });
	gameData.rectPos = glm::clamp(gameData.rectPos, glm::vec2{0,0}, glm::vec2{w - 100,h - 40});

	// Define o tamanho da imagem

	//std::string s = std::to_string(points);
	//char const* stringPoints = s.c_str();
	if (option == 0) {
		definitions::renderBaseMenu();
	}else {
		definitions::renderer.renderRectangle({ gameData.buildingA, buildingSize.sizeX, buildingSize.sizeY }, definitions::building);
		definitions::renderer.renderRectangle({ gameData.buildingB, buildingSize.sizeX, buildingSize.sizeY }, definitions::building);
		definitions::renderer.renderRectangle({ gameData.ground, w, h / 12 }, definitions::ground);
		definitions::renderer.renderRectangle({ gameData.bridge, w / 7, h / 12 }, definitions::bridge);
		definitions::renderer.renderRectangle({gameData.rectPos, helicopterSizes.sizeX, helicopterSizes.sizeY}, definitions::t);
		definitions::renderPoints(points);
	}
	//definitions::renderer.renderText({30, 30}, stringPoints, definitions::font, Colors_White, .5);
	//definitions::renderer.renderText({ 250, 30 }, "Selecione o modo de jogo", definitions::font, Colors_White, .3);





	
	
	
	
	////////////////////////// Logica do anti aereo 

	// L�gica de movimento do antiaircraft A
	static bool moveRightA = true;

	if (moveRightA)
	{
		gameData.antiaircraftAPos.x += deltaTime * 50;
		if (gameData.antiaircraftAPos.x > w - antiaircraftSizes.sizeX)
		{
			moveRightA = false;
		}
	}
	else
	{
		gameData.antiaircraftAPos.x -= deltaTime * 50;
		if (gameData.antiaircraftAPos.x < 0)
		{
			moveRightA = true;
		}
	}

	// Renderize o antiaircraft A
	definitions::renderer.renderRectangle({ gameData.antiaircraftAPos, antiaircraftSizes.sizeX, antiaircraftSizes.sizeY }, definitions::antiair);

	// L�gica de movimento do antiaircraft B
	static float moveSpeedB = 200.0f;
	static bool moveRightB = true;

	if (moveRightB)
	{
		gameData.antiaircraftBPos.x += deltaTime * moveSpeedB;
		if (gameData.antiaircraftBPos.x > w - antiaircraftSizes.sizeX)
		{
			moveRightB = false;
			moveSpeedB = std::uniform_real_distribution<float>(100, 300)(std::mt19937(std::random_device()()));
		}
	}
	else
	{
		gameData.antiaircraftBPos.x -= deltaTime * moveSpeedB;
		if (gameData.antiaircraftBPos.x < 0)
		{
			moveRightB = true;
			moveSpeedB = std::uniform_real_distribution<float>(100, 300)(std::mt19937(std::random_device()()));
		}
	}

	// Renderize o antiaircraft B
	definitions::renderer.renderRectangle({ gameData.antiaircraftBPos, antiaircraftSizes.sizeX, antiaircraftSizes.sizeY }, definitions::antiair);

	// Defina a coordenada Y dos antiaircrafts para ser a mesma
	gameData.antiaircraftAPos.y = 400;
	gameData.antiaircraftBPos.y = 400;

	/////////////////////////// Fim da logica do anti aereo











	definitions::renderer.flush();

	//ImGui::ShowDemoWindow();


	return true;
#pragma endregion

}

//This function might not be be called if the program is forced closed
void closeGame()
{

	//saved the data.
	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
