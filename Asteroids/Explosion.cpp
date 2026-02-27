#include "Explosion.h"
#include <iostream>

sf::Texture Explosion::s_dustTexture;
sf::Texture Explosion::s_explosionTexture;
 
Explosion::Explosion() :m_active{ false }
{

}


Explosion::~Explosion()
{
}

void Explosion::render(sf::RenderWindow & t_window)
{
	if (m_active)
	{
		t_window.draw(m_explosionSprite);
	}
}

void Explosion::update(sf::Time t_deltaTime)
{	
	if (m_active)
	{
		m_frame++;
		if (m_frame > EXPLOSION_LAST_FRAME)
		{
			m_active = false;
		}
	}
	int col = m_frame % 10;
	int row = m_frame / 10;
	m_explosionSprite.setTextureRect(sf::IntRect{ sf::Vector2i{ col * m_frameSize, row * m_frameSize},sf::Vector2i{ m_frameSize, m_frameSize} });

}

void Explosion::loadContent()
{
	if (!Explosion::s_dustTexture.loadFromFile("assets\\images\\dust.png"))
	{
		std::cout << "problem loading dust " << std::endl;
	}
	if (!Explosion::s_explosionTexture.loadFromFile("assets\\images\\explosion.png"))
	{
		std::cout << "problem loading explosion " << std::endl;
	}
	

}

void Explosion::activate(sf::Vector2f t_location, animation t_type)
{
	switch (t_type)
	{
	case animation::Dust:
		m_explosionSprite.setTexture(s_dustTexture,true);
		m_frameSize = 64;
		break;
	case animation::Explosion:
		m_explosionSprite.setTexture(s_explosionTexture,true);
		m_frameSize = 128;
		break;
	default:
		break;
	}
	m_frame = 0;
	m_explosionSprite.setTextureRect(sf::IntRect{ sf::Vector2i{0,0},sf::Vector2i{m_frameSize,m_frameSize} });
	m_active = true;
	m_explosionSprite.setPosition(t_location - sf::Vector2f{ static_cast<float>(m_frameSize)/2.0f,static_cast<float>(m_frameSize) / 2.0f });
}
