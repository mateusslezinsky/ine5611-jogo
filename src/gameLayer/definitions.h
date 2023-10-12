#include "gameLayer.h"
#include "platformInput.h"
#include <gl2d/gl2d.h>

namespace definitions {
	struct HelicopterSizes {
		int sizeX = 90;
		int sizeY = 30;
	}helicopterSizes;

	struct AntiaircraftSizes
	{
		int sizeX = 120;
		int sizeY = 50;
	}
	antiaircraftSizes;

	struct MenuItemsPos {
		int easyCornerXLeft = 230;
		int easyCornerXRight = 270;
		int easyCornerYLeft = 55;
		int easyCornerYRight = 70;
		//bool easyPosX = mousePos.mouseX >= easyCornerXLeft && mousePos.mouseX < easyCornerXRight;
		//bool easyPosY = mousePos.mouseY >= easyCornerYLeft && mousePos.mouseY < easyCornerYRight;
	}menuItemsPos;

	struct MousePos {
		int mouseX;
		int mouseY;
	} mousePos;

	void setMousePosition() {
		//Pega x e y do mouse
		mousePos.mouseX = platform::getRelMousePosition().x;
		mousePos.mouseY = platform::getRelMousePosition().y;
	}
	
	gl2d::Renderer2D renderer;
	gl2d::Font font;
	gl2d::Texture t;
	gl2d::Texture at;
	gl2d::Texture antiair;


	void renderBaseMenu()
	{
		renderer.renderText({ 250, 30 }, "Selecione o modo de jogo", font, Colors_White, .4);
		renderer.renderText({ 250, 60 }, "Facil", font, Colors_White, .3);
		renderer.renderText({ 250, 90 }, "Normal", font, Colors_White, .3);
		renderer.renderText({ 250, 120 }, "Dificil", font, Colors_White, .3);
	}
	void handleSelectMenu() 
	{
		
	}

	void renderPoints(int currentPoints)
	{
		std::string s = std::to_string(currentPoints);
		char const* stringPoints = s.c_str();
		renderer.renderText({ 30, 30 }, stringPoints, font, Colors_White, .5);
	}
}