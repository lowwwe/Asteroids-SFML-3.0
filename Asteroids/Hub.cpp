#include "Hub.h"
#include <iostream>
#include "Game.h"



Hub::Hub()
{
	if (!m_backgroundTexture.loadFromFile("ASSETS\\IMAGES\\menu.jpg"))
	{
		std::cout << "problem with menu" << std::endl;
	}
	m_backgroundSprite.setTexture(m_backgroundTexture,true);
	m_backgroundSprite.setPosition(sf::Vector2f{ 0.0f, 0.0f });
}


Hub::~Hub()
{
}

void Hub::render(sf::RenderWindow & t_window)
{
	t_window.draw(m_backgroundSprite);
	t_window.draw(m_mapText);
	t_window.draw(m_hangerText);
	t_window.draw(m_marketText);
	t_window.draw(m_helpText);
	t_window.draw(m_exitText);
}

void Hub::update(sf::Time deltaTime, sf::RenderWindow &window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	m_mapText.setFillColor(sf::Color::White);
	m_hangerText.setFillColor(sf::Color::White);
	m_marketText.setFillColor(sf::Color::White);
	m_helpText.setFillColor(sf::Color::White);
	m_exitText.setFillColor(sf::Color::White);
	m_currentRegion = HubRegion::None;
	if (mousePosition.y < 400)
	{
		if (mousePosition.x > 370)
		{
			if (mousePosition.y > 200)
			{
				m_helpText.setFillColor(sf::Color::Yellow);
				m_currentRegion = HubRegion::Help;
			}
			else
			{
				m_hangerText.setFillColor(sf::Color::Yellow);
				m_currentRegion = HubRegion::Hanger;
			}
		}
		else
		{
			if (mousePosition.y > 200)
			{
				m_marketText.setFillColor(sf::Color::Yellow);
				m_currentRegion = HubRegion::Market;
			}
			else
			{
				m_mapText.setFillColor(sf::Color::Yellow);
				m_currentRegion = HubRegion::Map;
			}
		}
	}
	if (mousePosition.y > 500 && mousePosition.y < 530
		&& mousePosition.x > 150 && mousePosition.x < 250)
	{
		m_exitText.setFillColor(sf::Color::Yellow);
		m_currentRegion = HubRegion::Exit;
	}
}

void Hub::processEvents(const std::optional<sf::Event> t_event)
{

	if (t_event->is<sf::Event::MouseButtonReleased>())
	{

		switch (m_currentRegion)
		{
		case HubRegion::Hanger:
			Game::s_currentGameState = GameState::Hanger;
			break;
		case HubRegion::Map:
			Game::s_currentGameState = GameState::Map;
			break;
		case HubRegion::Market:
			Game::s_currentGameState = GameState::Market;
			break;
		case HubRegion::Help:
			Game::s_currentGameState = GameState::Help;
			break;		
		case HubRegion::Exit:
			Game::s_currentGameState = GameState::Exit;
		}
	}
}

void Hub::initialise(sf::Font & font)
{
	m_font = font;
	setupText(m_mapText, "MAP", { 150,100 });
	setupText(m_hangerText, "HANGER", { 500,100 });
	setupText(m_marketText, "MARKET", { 150,300 });
	setupText(m_helpText, "HELP", { 500,300 });
	setupText(m_exitText, "EXIT", { 150,500 });
}
void  Hub::setupText(sf::Text &text, std::string string, sf::Vector2f position)
{
	text.setFont(m_font);
	text.setCharacterSize(24);
	text.setString(string);
	text.setFillColor(sf::Color::White);
	text.setPosition(position);
}
