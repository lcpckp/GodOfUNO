#pragma once
#include<SFML/Graphics.hpp>

class State
{
private:
public:
	State();
	virtual int requestingClose() = 0;
	bool requestingMusic();
	virtual std::unique_ptr<State> getNextState() = 0;
	virtual bool hasNextState() = 0;
	virtual void pollEvents(sf::RenderWindow& mainWindow) = 0;
	virtual void update(sf::Time dt) = 0;
	virtual void render(sf::RenderWindow& mainWindow) = 0;

	bool closeRequest;
	bool musicRequest;
};

