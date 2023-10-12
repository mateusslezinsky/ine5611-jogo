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

class HelicopterSize {
public :
	int sizeX = 90;
	int sizeY = 30;
};



struct GameData
{
	// Posição dos objetos na tela
	glm::vec2 rectPos = {0,0};
	glm::vec2 rect = { 0,0 };

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

	definitions::at.loadFromFile(RESOURCES_PATH "building.png", false);
	definitions::t.loadFromFile(RESOURCES_PATH "test.png", false);
	definitions::font.createFromFile(RESOURCES_PATH "roboto_black.ttf");
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

	// Declara as strings de posição (x e y) e converte-as do objeto original que era em outro formato (possivelmente float ou double)
	std::string playerPosx = std::to_string(gameData.rectPos.x);
	std::string playerPosy = std::to_string(gameData.rectPos.y);

	//if (int(gameData.rectPos.x) == 0 || int(gameData.rectPos.x) == (platform::getFrameBufferSizeX()) - 100) {
	//	helicopterSizes.sizeX= 0;
	//}
	if (int(gameData.rectPos.y) == 0 || int(gameData.rectPos.y) == (platform::getFrameBufferSizeY())) {
		std::exit(0);
	}
	
	if (int(gameData.rectPos.x) == 350) {
		points += 10;
	}

	if (platform::isKeyHeld(platform::Button::Left))
	{
		// Define a velocidade do objeto para esquerda
		gameData.rectPos.x -= deltaTime * 200;
		// Imprime no console
		//std::cout << "x: " + playerPosx + " ";
		//std::cout << "y: " + playerPosy + "\n";
	}
	if (platform::isKeyHeld(platform::Button::Right))
	{
		// Define a velocidade do objeto para direita
		gameData.rectPos.x += deltaTime * 200;
		// Imprime no console
		//std::cout << "x: " + playerPosx + " ";
		//std::cout << "y: " + playerPosy + "\n";
	}
	if (platform::isKeyHeld(platform::Button::Up))
	{
		// Define a velocidade do objeto para cima
		gameData.rectPos.y -= deltaTime * 200;
		// Imprime no console
		//std::cout << "x: " + playerPosx + " ";
		//std::cout << "y: " + playerPosy + "\n";
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
		if (posX && posY) {
			std::cout << "Pressionou Facil \n";
			option = 1;
		}
	}


	gameData.rect = glm::clamp(gameData.rect, glm::vec2{ 0,0 }, glm::vec2{ w - 100,h - 100 });
	gameData.rectPos = glm::clamp(gameData.rectPos, glm::vec2{0,0}, glm::vec2{w - 100,h});

	// Define o tamanho da imagem

	//std::string s = std::to_string(points);
	//char const* stringPoints = s.c_str();
	if (option == 0) {
		definitions::renderBaseMenu();
	}else {
		definitions::renderer.renderRectangle({ gameData.rect, 150, 150 }, definitions::at);
		definitions::renderer.renderRectangle({gameData.rectPos, helicopterSizes.sizeX, helicopterSizes.sizeY}, definitions::t);
		definitions::renderPoints(points);
	}
	//definitions::renderer.renderText({30, 30}, stringPoints, definitions::font, Colors_White, .5);
	//definitions::renderer.renderText({ 250, 30 }, "Selecione o modo de jogo", definitions::font, Colors_White, .3);

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
