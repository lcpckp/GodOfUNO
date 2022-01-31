#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include "UnoCard.h"
#include "State.h"
#include "Resources.h"
#include "UnoGame.h"
#include "StartMenu.h"
#include "GameEngine.h"
#include<iostream>
#include<vector>
#include<chrono>
#include<thread>
#include<stack>
#include<map>
#include<memory>

class GameEngine;

class PauseMenu : public State
{
private:
	sf::Font font;
	sf::Text reallyQuit;
	sf::Text backToGame;
	sf::Text backToMenu;

	sf::Sprite ESCbutton;
	sf::Sprite Qbutton;
	sf::Sprite Bbutton;

	sf::Vector2i mouseClickPos;
	sf::Event ev;
	
	sf::VideoMode pauseMenuVideoMode;

	int closeRequest;
	bool menuRequest;
	GameEngine* mainGame;

public:
	PauseMenu(sf::VideoMode videomode, GameEngine* mg);

	int requestingClose();

	void pollEvents(sf::RenderWindow& window);
	void update(sf::Time dt);
	void render(sf::RenderWindow& window);
	std::unique_ptr<State> getNextState();
	bool hasNextState();
};

