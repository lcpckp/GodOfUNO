#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>


class Resources
{
private:

public:
	Resources(sf::VideoMode videoMode1);

	//All Colors in Game
	static sf::Color availableCardColors[];
	
	//Card Settings
	static int CARD_WIDTH;
	static int CARD_HEIGHT;
	static int CARD_SPACING;
	static int CARD_OFFSET_X;
	static sf::Font CARD_FONT;
	static int FONT_SIZE_SMALL;
	static int FONT_SIZE_NORMAL;
	static int FONT_SIZE_LARGE;
	static int FONT_SIZE_XL;

	//Window Settings
	static int SCREEN_SECTIONS_RESOLUTION;
	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;

	//Computer Settings
	static int COMPUTER_DELAY_MS;
	static int DELAY_BETWEEN_GAME_MS;
	static int DELAY_BETWEEN_GAMES_DEFAULT;
	static int DELAY_EVERY_NUM_GAMES_MS;
	static int PAUSE_EVERY_NUM_GAMES;
	enum COMPUTER_DELAY_SPEEDS { COMPUTER_DELAY_GRANDMA = 4000, COMPUTER_DELAY_SLOW = 1250, COMPUTER_DELAY_BRISK = 750, COMPUTER_DELAY_FAST = 150, COMPUTER_DELAY_EXTREME = 10, COMPUTER_DELAY_INSANE = 1, COMPUTER_DELAY_NONE = 0 };
	//Game Settings
	static int NUM_PLAYERS;
	static bool autoPlay;

	static sf::Texture Qtexture;
	static sf::Texture Btexture;
	static sf::Texture ESCtexture;
	static sf::Texture Mtexture;
	static sf::Texture Atexture;
	static sf::Texture Rtexture;
};

