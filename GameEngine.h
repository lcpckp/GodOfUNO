#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<memory>
#include<stack>
#include "UnoGame.h"
#include "State.h"
#include "Resources.h"
#include "StartMenu.h"


class GameEngine : public State
{
private:
	
	sf::Font CARD_FONT;

	sf::Vector2i mouseClickPos;
	sf::Event ev;
	sf::VideoMode videoMode1;

	//Uno Game Container
	std::unique_ptr<UnoGame> game;
	//Game Status
	int selectedDifficulty;
	bool shuffleRequest;
	bool pauseRequest;
	bool gameEndScriptHasRun;
	bool gameStarted;
	
	void drawGame(sf::RenderWindow& window);

	std::stack<std::unique_ptr<State>> mainStackRef;

	sf::Text reshuffleText;
	sf::Text automaticText;
	sf::Text escapeMenuText;
	sf::Text musicText;
	sf::Text gameCountText;
	
	sf::Sprite escapeMenuIcon;
	sf::Sprite reshuffleTextIcon;
	sf::Sprite automaticTextIcon;
	sf::Sprite musicTextIcon;
	
public:
	
	GameEngine();
	GameEngine(sf::VideoMode videomode, int diff);

	int requestingClose();
	bool hasNextState();
	std::unique_ptr<State> getNextState();
	void pollEvents(sf::RenderWindow& mainWindow);
	void update(sf::Time dt);
	void render(sf::RenderWindow& window);

	void depositUnoGame(std::unique_ptr<UnoGame> ug);
	std::unique_ptr<UnoGame> getUnoGame();

	sf::Clock resetGameTimer;
};
