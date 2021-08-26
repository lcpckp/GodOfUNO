//Normal Stuff
#include<iostream>
#include<vector>
#include<chrono>
#include<thread>

//Class Headers
#include"UnoCard.h"
#include"Player.h"
#include"UnoGame.h"
#include"GameEngine.h"

//SFML Libraries
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Network.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>

//config.ini
const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 1600;


int main()
{
	//Random Seed
	srand(time(NULL));

	//Set up Render window and Event Listener
	sf::VideoMode videoMode1(WINDOW_WIDTH, WINDOW_HEIGHT);
	sf::RenderWindow mainWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "God of UNO");

	GameEngine game1;

	sf::Clock clock;
	const sf::Time timePerFrame = sf::seconds(1.f / 60);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mainWindow.isOpen())
	{
		game1.pollEvents(mainWindow);
		timeSinceLastUpdate += clock.restart();
		game1.updateGame(timeSinceLastUpdate);
		game1.renderGame(mainWindow);
	}

	return 0;


	



}














/*
void drawWinText(sf::RenderWindow* window, UnoGame& game)
{
	sf::Text winText;
	sf::Text scoreText;
	sf::Text totalScoreText;
	sf::Font font;

	sf::Text::Style scoreSubtitle_style;
	sf::Text::Style winTitle_style;
	font.loadFromFile("arialbd.ttf");

	int winnerIndex = game.over() - 1;
	std::string winnerName = game.getPlayers()->at(winnerIndex).getName();
	std::string loserName = game.getPlayers()->at(!winnerIndex).getName();


	winText.setFont(font);
	winText.setCharacterSize(50);
	winText.setPosition(sf::Vector2f(5, 5));
	winText.setFillColor(sf::Color::White);

	winText.setString((winnerName + " wins!"));

	scoreText.setFont(font);
	scoreText.setCharacterSize(30);
	scoreText.setPosition(sf::Vector2f(5, 60));
	scoreText.setFillColor(sf::Color::White);

	scoreText.setString(winnerName + " gets " + std::to_string(game.getLatestScore()) + " points.");

	totalScoreText.setFont(font);
	totalScoreText.setCharacterSize(30);
	totalScoreText.setPosition(sf::Vector2f(5, 100));
	totalScoreText.setFillColor(sf::Color::White);

	totalScoreText.setString(winnerName + " has " + std::to_string(game.getPlayers()->at(winnerIndex).getScore()) + " total points.\n" + loserName + " has " + std::to_string(game.getPlayers()->at(!winnerIndex).getScore()) + " total points.\n");



	window->draw(winText);
	window->draw(scoreText);
	window->draw(totalScoreText);
}
*/