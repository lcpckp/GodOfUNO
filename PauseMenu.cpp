#include "PauseMenu.h"


PauseMenu::PauseMenu(sf::VideoMode videomode, GameEngine* mg)
{
    closeRequest = false;
    pauseMenuVideoMode = videomode;
    mainGame = std::move(mg);

    
    //set sprite textures
    Qbutton.setTexture(Resources::Qtexture);
    ESCbutton.setTexture(Resources::ESCtexture);
    Bbutton.setTexture(Resources::Btexture);

    //set initial pos
    Qbutton.setPosition(Resources::WINDOW_WIDTH / 2, Resources::WINDOW_HEIGHT / 2 + 110);
    ESCbutton.setPosition(Resources::WINDOW_WIDTH / 2, Resources::WINDOW_HEIGHT / 2);
    Bbutton.setPosition(Resources::WINDOW_WIDTH / 2, Resources::WINDOW_HEIGHT / 2 + 55);

	//set strings
    reallyQuit.setFont(Resources::CARD_FONT);
	reallyQuit.setString("Quit to Desktop");
	reallyQuit.setCharacterSize(Resources::FONT_SIZE_XL);

    backToGame.setFont(Resources::CARD_FONT);
    backToGame.setString("Back to Game");
    backToGame.setCharacterSize(Resources::FONT_SIZE_XL);

    backToMenu.setFont(Resources::CARD_FONT);
    backToMenu.setString("Change Difficulty");
    backToMenu.setCharacterSize(Resources::FONT_SIZE_XL);

    menuRequest = false;
}

std::unique_ptr<State> PauseMenu::getNextState()
{
    return nullptr;//std::move(mainGame);
}

bool PauseMenu::hasNextState()
{
    return false;
}

int PauseMenu::requestingClose()
{
    if (closeRequest > 1)
    {
        mainGame->getUnoGame()->resetPlayers();
        mainGame->getUnoGame().reset();
    }
	return closeRequest;
}

void PauseMenu::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(30, 30, 30));
    
    window.draw(reallyQuit);
    window.draw(backToGame);
    window.draw(backToMenu);
    window.draw(Qbutton);
    window.draw(ESCbutton);
    window.draw(Bbutton);

    window.display();
}

void PauseMenu::pollEvents(sf::RenderWindow& window)
{
    while (window.pollEvent(ev))
    {
        switch (ev.type)
        {
        case sf::Event::MouseButtonPressed:
            //mouseClickPos = sf::Mouse::getPosition(window); //maybe turn on later
            //std::cout << "[MOUSE] mouseClickPos.x : " << mouseClickPos.x << " mouseClickPos.y : " << mouseClickPos.y << std::endl;
            break;
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if (ev.key.code == sf::Keyboard::Escape)
                closeRequest = 1;
            if (ev.key.code == sf::Keyboard::B)
                closeRequest = 2;
            if (ev.key.code == sf::Keyboard::Q)
                closeRequest = 3;
            break;
        }
    }
}

void PauseMenu::update(sf::Time dt)
{
    sf::Vector2f buttonTargetPosition = sf::Vector2f(Resources::WINDOW_WIDTH / 2 - 100, Resources::WINDOW_HEIGHT / 2 + 90);
    sf::Vector2f buttonDistanceVector = buttonTargetPosition - Qbutton.getPosition();
    float buttonLength = std::sqrt(buttonDistanceVector.x * buttonDistanceVector.x + buttonDistanceVector.y * buttonDistanceVector.y);

    if (buttonLength < 1)
    {
        Qbutton.setPosition(buttonTargetPosition);

    }
    else
    {
        Qbutton.setPosition(Qbutton.getPosition() + ((buttonDistanceVector * 3.f) * (float)dt.asSeconds()));
    }

    buttonTargetPosition = sf::Vector2f(Resources::WINDOW_WIDTH / 2 - 100, Resources::WINDOW_HEIGHT / 2 - 30);
    buttonDistanceVector = buttonTargetPosition - ESCbutton.getPosition();
    buttonLength = std::sqrt(buttonDistanceVector.x * buttonDistanceVector.x + buttonDistanceVector.y * buttonDistanceVector.y);

    if (buttonLength < 1)
    {
        ESCbutton.setPosition(buttonTargetPosition);

    }
    else
    {
        ESCbutton.setPosition(ESCbutton.getPosition() + ((buttonDistanceVector * 6.f) * (float)dt.asSeconds()));
    }

    buttonTargetPosition = sf::Vector2f(Resources::WINDOW_WIDTH / 2 - 100, Resources::WINDOW_HEIGHT / 2 + 30);
    buttonDistanceVector = buttonTargetPosition - Bbutton.getPosition();
    buttonLength = std::sqrt(buttonDistanceVector.x * buttonDistanceVector.x + buttonDistanceVector.y * buttonDistanceVector.y);

    if (buttonLength < 1)
    {
        Bbutton.setPosition(buttonTargetPosition);

    }
    else
    {
        Bbutton.setPosition(Bbutton.getPosition() + ((buttonDistanceVector * 4.f) * (float)dt.asSeconds()));
    }

    reallyQuit.setPosition(Qbutton.getPosition() + sf::Vector2f(60, -15));
    backToGame.setPosition(ESCbutton.getPosition() + sf::Vector2f(60, -15));
    backToMenu.setPosition(Bbutton.getPosition() + sf::Vector2f(60, -15));
}