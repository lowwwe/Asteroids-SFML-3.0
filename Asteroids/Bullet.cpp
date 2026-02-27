#include "Bullet.h"

sf::Texture Bullet::s_bulletTexture;
int Bullet::s_bulletLife = 180;

Bullet::Bullet()
{
	//m_bulletSprite.setTexture(s_bulletTexture,true);
	
	m_alive = false;
	m_framesLeft = 240;
	
}


Bullet::~Bullet()
{
}

void Bullet::render(sf::RenderWindow & t_window)
{
	if (m_alive)
	{
		t_window.draw(m_bulletSprite);
	}
}

void Bullet::update(sf::Time t_delta)
{
	if (m_alive)
	{
		m_location += m_velocity;
		if (m_framesLeft-- < 0)
		{
			m_alive = false;
		}
		screenWrap();
		m_bulletSprite.setPosition(m_location);
	}
}

void Bullet::reStart(int t_size, MyVector2D t_loaction, MyVector2D t_velocity, float t_heading, bool t_friendly)
{
	m_bulletSprite.setTexture(s_bulletTexture, true);
	m_bulletSprite.setOrigin(sf::Vector2f{ 8.0f, 8.0f });
	m_alive = true;
	m_friendly = t_friendly;
	m_location = t_loaction;
	m_velocity = t_velocity *5.0f;
	m_framesLeft = s_bulletLife;
	m_bulletSprite.setRotation(sf::degrees(t_heading + 90.0f));
	m_bulletSprite.setPosition(m_location);
	
}



void Bullet::screenWrap()
{
	
	int const OFFSET = 32; // ship radius
	if (m_location.y < -OFFSET)
	{
		
		m_alive = false;
	}
	if (m_location.y  > 640 + OFFSET)
	{
	
		m_alive = false;
	}
	if (m_location.x < -OFFSET)
	{
		
		m_alive = false;
	}
	if (m_location.x > 800 + OFFSET)
	{
	
		m_alive = false;
	}
}
