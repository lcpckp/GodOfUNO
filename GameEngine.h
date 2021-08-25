#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<iostream>
#include "UnoGame.h"
#include "UnoCard.h"

class GameEngine
{
private:
	const int CARD_WIDTH = 50;
	const int CARD_HEIGHT = 80;
	const int CARD_SPACING = 5;
	const int WINDOW_HEIGHT = 600;
	const int WINDOW_WIDTH = 1200;
	const int SCREEN_SECTIONS_RESOLUTION = 8;
	sf::Font CARD_FONT;

	UnoGame game;

	sf::Vector2i mouseClickPos;
	sf::Event ev;

	bool shuffleRequest;
	bool scoreTallied;
	void printCards(sf::RenderWindow* window, std::vector<UnoCard>& d, int x, int y);
	void printCardsDeck(sf::RenderWindow* window, std::vector<UnoCard>& d, int x, int y);
	void printCardsDiscard(sf::RenderWindow* window, std::vector<UnoCard>& d, int x, int y);
	void drawButtons(sf::RenderWindow* window, UnoGame& game);
	
public:
	GameEngine();
	void pollEvents(sf::RenderWindow& mainWindow);
	void updateGame();
	void drawGame(sf::RenderWindow* window);
	void renderGame(sf::RenderWindow& window);
};

