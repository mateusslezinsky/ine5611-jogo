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


struct GameData
{
	glm::vec2 rectPos = {100,100};
	glm::vec2 rect = { 100,100 };

}gameData;

gl2d::Renderer2D renderer;
gl2d::Texture t;
gl2d::Texture at;
gl2d::Font f;

bool initGame()
{
	//initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	//loading the saved data. Loading an entire structure like this makes savind game data very easy.
	platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

	at.loadFromFile(RESOURCES_PATH "test.jpg", true);
	t.loadFromFile(RESOURCES_PATH "test.png", true);
	f.createFromFile(RESOURCES_PATH "roboto_black.ttf");
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

	renderer.updateWindowMetrics(w, h);

#pragma endregion


	if (platform::isKeyHeld(platform::Button::Left))
	{
		// Define a velocidade do objeto para esquerda
		gameData.rectPos.x -= deltaTime * 100;
		
	}
	if (platform::isKeyHeld(platform::Button::Right))
	{
		// Define a velocidade do objeto para direita
		gameData.rectPos.x += deltaTime * 100;
	}
	if (platform::isKeyHeld(platform::Button::Up))
	{
		// Define a velocidade do objeto para cima
		gameData.rectPos.y -= deltaTime * 100;
	}
	if (platform::isKeyHeld(platform::Button::Down))
	{
		// Define a velocidade do objeto para baixo
		gameData.rectPos.y += deltaTime * 100;
	}

	gameData.rectPos = glm::clamp(gameData.rectPos, glm::vec2{0,0}, glm::vec2{w - 100,h - 100});
	gameData.rect = glm::clamp(gameData.rect, glm::vec2{ 0,0 }, glm::vec2{ w - 100,h - 100 });
	// Define o tamanho da imagem
	renderer.renderRectangle({gameData.rectPos, 180, 50}, t);
	renderer.renderRectangle({ gameData.rect, 100, 100 }, at);

	//renderer.renderText({200, 200}, "Hello my first game", f, Colors_White);


	renderer.flush();



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
