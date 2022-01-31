#include "Resources.h"
sf::Color Resources::availableCardColors[] = { sf::Color(180, 0, 0), sf::Color(220, 170, 0), sf::Color(0, 180, 0), sf::Color(0, 0, 180), sf::Color(140, 0, 180), sf::Color(220, 100, 0),
												sf::Color(120, 120, 120), sf::Color(255, 255, 255), sf::Color(121, 121, 121), sf::Color(254, 254, 254) };

enum COMPUTER_DELAY_SPEEDS { COMPUTER_DELAY_GRANDMA = 4000, COMPUTER_DELAY_SLOW = 1250, COMPUTER_DELAY_BRISK = 750, COMPUTER_DELAY_FAST = 150, COMPUTER_DELAY_EXTREME = 10, COMPUTER_DELAY_INSANE = 1, COMPUTER_DELAY_NONE = 0 };


int Resources::CARD_WIDTH;
int Resources::CARD_HEIGHT;
int Resources::CARD_SPACING;
int Resources::CARD_OFFSET_X;
sf::Font Resources::CARD_FONT;

int Resources::FONT_SIZE_SMALL;
int Resources::FONT_SIZE_NORMAL;
int Resources::FONT_SIZE_LARGE;
int Resources::FONT_SIZE_XL;

int Resources::SCREEN_SECTIONS_RESOLUTION;
int Resources::WINDOW_WIDTH;
int Resources::WINDOW_HEIGHT;


int Resources::COMPUTER_DELAY_MS;
int Resources::DELAY_BETWEEN_GAME_MS = 0;
int Resources::DELAY_BETWEEN_GAMES_DEFAULT = 500;
int Resources::DELAY_EVERY_NUM_GAMES_MS = 5000;
int Resources::PAUSE_EVERY_NUM_GAMES = 50;

int Resources::NUM_PLAYERS;
bool Resources::autoPlay;

sf::Texture Resources::Qtexture;
sf::Texture Resources::ESCtexture;
sf::Texture Resources::Btexture;
sf::Texture Resources::Mtexture;
sf::Texture Resources::Atexture;
sf::Texture Resources::Rtexture;


Resources::Resources(sf::VideoMode videoMode1)
{
	WINDOW_WIDTH = videoMode1.width;
	WINDOW_HEIGHT = videoMode1.height;
	// All of the sizes of cards, positions on screen, etc. are based on pixels and were designed on a 1080p resolution.
	// The SCALING_FACTOR variable should adjust for different resolutions from this.
	// Theoretically I could adjust it in both directions but I think as long as the aspect ratio is the same, this won't matter, so I've taken a shortcut for now.
	float SCALING_FACTOR = WINDOW_HEIGHT / 1080.0f;
	CARD_FONT.loadFromFile("arialbd.ttf");
	CARD_WIDTH = 65 * SCALING_FACTOR;
	CARD_HEIGHT = 100 * SCALING_FACTOR;
	CARD_SPACING = 10 * SCALING_FACTOR;
	CARD_OFFSET_X = CARD_WIDTH + CARD_SPACING;
	SCREEN_SECTIONS_RESOLUTION = 8;
	FONT_SIZE_SMALL = 25 * SCALING_FACTOR;
	FONT_SIZE_NORMAL = 35 * SCALING_FACTOR;
	FONT_SIZE_LARGE = 45 * SCALING_FACTOR;
	FONT_SIZE_XL = 60 * SCALING_FACTOR;

	NUM_PLAYERS = 2;
	autoPlay = false;

	
	COMPUTER_DELAY_MS = COMPUTER_DELAY_BRISK;

	ESCtexture.loadFromFile("Sprites\\ESC.png");
	Qtexture.loadFromFile("Sprites\\Q.png");
	Btexture.loadFromFile("Sprites\\B.png");
	Mtexture.loadFromFile("Sprites\\M.png");
	Atexture.loadFromFile("Sprites\\A.png");
	Rtexture.loadFromFile("Sprites\\R.png");
}