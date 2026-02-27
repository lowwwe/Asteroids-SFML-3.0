#include "Splash.h"
#include <iostream>
#include "Game.h"


Splash::Splash()
{
	if (!m_backgroundTexture.loadFromFile("ASSETS\\IMAGES\\SPLASH.jpg"))
	{
		std::cout << "problem with splash" << std::endl;
	}
	m_backgroundSprite.setTexture(m_backgroundTexture,true);
	m_backgroundSprite.setPosition(sf::Vector2f{ 0.0f, 0.0f });
}


Splash::~Splash()
{
}

void Splash::render(sf::RenderWindow & window)
{
	window.draw(m_backgroundSprite);
}

void Splash::update(sf::Time deltaTime)
{
}

void Splash::processEvents(const std::optional<sf::Event> t_event)
{
	if (t_event->is<sf::Event::KeyPressed>()) //user pressed a key
	{

		Game::s_currentGameState = GameState::Hub;
	}
	if (t_event->is<sf::Event::MouseButtonPressed>()) // user presses the mouse
	{

		Game::s_currentGameState = GameState::Hub;
	}
}
