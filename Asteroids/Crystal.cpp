#include "Crystal.h"
#include <iostream>



sf::Texture Crystal::s_crystalTexture;

Crystal::Crystal()
{
}


Crystal::~Crystal()
{
}

void Crystal::render(sf::RenderWindow & t_window)
{
	
	if (m_active)
	{
		t_window.draw(m_crystalSprite);
#ifdef _DEBUG
		sf::CircleShape dot{ 2.0f };
		dot.setPosition(m_location);
		t_window.draw(dot);
		dot.setRadius(16);
		dot.setPosition(m_location - sf::Vector2f{ 16.0f,16.0f});
		dot.setFillColor(sf::Color::Transparent);
		dot.setOutlineColor(sf::Color::Green);
		dot.setOutlineThickness(1.0f);
		t_window.draw(dot);
#endif
	}
}

void Crystal::update(sf::Time t_deltaTime)
{
}

void Crystal::loadContent()
{
	if (!Crystal::s_crystalTexture.loadFromFile("assets\\images\\gems.png"))
	{
		std::cout << "problem loading Crystal " << std::endl;
	}
}

void Crystal::activate(sf::Vector2f t_location, int t_type)
{
	m_crystalSprite.setTexture(s_crystalTexture);
	m_crystalSprite.setTextureRect(sf::IntRect{ sf::Vector2i{t_type * 32,0}, sf::Vector2i{32,32} });
	m_crystalSprite.setOrigin(sf::Vector2f{ 16.0f, 16.0f });
	m_crystalSprite.setPosition(t_location);
	m_type = t_type;
	m_location = t_location;
	m_active = true;
}
