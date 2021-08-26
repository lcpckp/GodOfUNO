#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<iostream>
#include<memory>
#include "UnoGame.h"
#include "UnoCard.h"

class GameEngine
{
private:
	
	sf::Font CARD_FONT;

	std::unique_ptr<UnoGame> game;

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
	void updateGame(sf::Time dt);
	void drawGame(sf::RenderWindow* window);
	void renderGame(sf::RenderWindow& window);
	static const int WINDOW_HEIGHT = 600;
	static const int WINDOW_WIDTH = 1600;
	static const int CARD_WIDTH = 50;
	static const int CARD_HEIGHT = 80;
	static const int CARD_SPACING = 5;
	static const int SCREEN_SECTIONS_RESOLUTION = 8;

};

