#include "Map.h"
#include <iostream>
#include <string.h>
#include "Game.h"




Map::Map()
{
	if (!m_gemsTexture.loadFromFile("ASSETS\\IMAGES\\gems.png"))
	{
		std::cout << "problem with gems" << std::endl;
	}
	m_gemsSprite.setTexture(m_gemsTexture,true);

	if (!m_menuTexture.loadFromFile("ASSETS\\IMAGES\\menuicon.png"))
	{
		std::cout << "problem with gems" << std::endl;
	}
	m_menuSprite.setTexture(m_menuTexture,true);
	m_menuSprite.setPosition({ 750, 18 });


	if (!m_backgroundTexture.loadFromFile("ASSETS\\IMAGES\\planet0.jpg"))
	{
		std::cout << "problem with menu" << std::endl;
	}
	m_backgroundSprite.setTexture(m_backgroundTexture,true);
	m_backgroundSprite.setPosition(sf::Vector2f{ 0.0f, 0.0f });
	
	
	for (size_t i = 0; i < maxPlanets; i++)
	{
		std::string file = "ASSETS\\IMAGES\\planet" + std::to_string(i) + ".png";
		if (!m_planetTextures[i].loadFromFile(file))
		{
			std::cout << "problem withf planet" << i << std::endl;
		}
		m_planetSprites[i].setTexture(m_planetTextures[i],true);
	}
}


Map::~Map()
{
}

void Map::render(sf::RenderWindow & window)
{
	window.draw(m_backgroundSprite);
	for (size_t i = 0; i < maxPlanets; i++)
	{
		if (Game::g_planets[i].active)
		{
			window.draw(m_planetSprites[i]);
		}
	}
	if (Game::s_currentPlanet > -1)
	{
		sf::Color colour{ 255,255,255,255 };
		if (Game::g_planets[Game::s_currentPlanet].active)
		{
			colour = sf::Color::Yellow;
		}
		m_pirates.setFillColor(colour);
		m_planetName.setFillColor(colour);
		m_gemProbability.setFillColor(colour);
		for (int i = 0; i < 5; i++)
		{
			m_gemsSprite.setTextureRect(sf::IntRect{ sf::Vector2i{i * 32,0},sf::Vector2i{32,32} });
			m_gemsSprite.setPosition({ 50.0f,140.0f + i * 40 });
			window.draw(m_gemsSprite);
			int gem =static_cast<int>( Game::g_planets[Game::s_currentPlanet].minerals[i] * 100.0);
			m_gemProbability.setString(": " + std::to_string(gem) + "%");
			m_gemProbability.setPosition({ 90.0f,140.0f + i * 40 });
			window.draw(m_gemProbability);
		}
		int pirates = static_cast<int>(Game::g_planets[Game::s_currentPlanet].pirates * 100.0);
		m_pirates.setString("Pirates : " + std::to_string(pirates) + "%");
		m_planetName.setString(Game::g_planets[Game::s_currentPlanet].name);
		window.draw(m_planetName);
		window.draw(m_pirates);
	}
	window.draw(m_menuSprite);
	
}

void Map::update(sf::Time deltaTime, sf::RenderWindow & window)
{
	Game::s_currentPlanet = -1;
	for (size_t i = 0; i < maxPlanets; i++)
	{
		if (sf::Mouse::getPosition(window).x > Game::g_planets[i].location.position.x
			&& sf::Mouse::getPosition(window).x < Game::g_planets[i].location.position.x + Game::g_planets[i].location.size.x
			&& sf::Mouse::getPosition(window).y > Game::g_planets[i].location.position.y
			&& sf::Mouse::getPosition(window).y < Game::g_planets[i].location.position.y + Game::g_planets[i].location.size.y)
		{
			Game::s_currentPlanet = i;
		
		}
	}
	if (m_mouseClick)
	{
		
		if (sf::Mouse::getPosition(window).x > 750 && sf::Mouse::getPosition(window).y < 50)
		{
			Game::s_currentGameState = GameState::Hub;
		}
		if (Game::s_currentPlanet != -1 && Game::g_planets[Game::s_currentPlanet].active)
		{
			//setupPlanet(m_currentPlanet);
			Game::s_gameplay.setupLevel(Game::s_currentPlanet);
			Game::s_currentGameState = GameState::Game;
			Game::s_music = Music::Level;
			//intitialiseLevel(m_currentPlanet;)
		}
	}
	m_mouseClick = false;
}

void Map::processEvents(const std::optional<sf::Event> t_event)
{
	if (t_event->is<sf::Event::MouseButtonReleased>())
	{	
		m_mouseClick = true;
	}
}

void Map::initialise(sf::Font & font)
{
	m_font = font;
	m_pirates.setFont(m_font);
	m_pirates.setCharacterSize(18);
	m_pirates.setFillColor(sf::Color::Yellow);
	m_pirates.setPosition({ 50,350 });
	
	m_planetName.setFont(m_font);
	m_planetName.setCharacterSize(24);
	m_planetName.setPosition({ 50,100 });
	m_planetName.setFillColor(sf::Color::Yellow);
	m_gemProbability.setFont(m_font);
	m_gemProbability.setCharacterSize(18);

}
