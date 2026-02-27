#include "Enemy.h"
#include <iostream>



Enemy::Enemy() : m_active {false}
{
	if (!m_shipTexture.loadFromFile("assets\\images\\enemyship64.png"))
	{
		std::cout << "problem loading enemy ship " << std::endl;
	}
	
	m_shipSprite.setTexture(m_shipTexture,true);
	m_shipSprite.setTextureRect(sf::IntRect{ sf::Vector2i{0,0},sf::Vector2i{64,64} });
	m_shipSprite.setPosition(sf::Vector2f{ 100.0,100.0 });
	m_shipSprite.setOrigin(sf::Vector2f{ 32.0f, 32.0f });
	m_shipSprite.rotate(sf::degrees(90));
	if (!m_shieldTexture.loadFromFile("assets\\images\\shield.png"))
	{
		std::cout << "problem loading shield" << std::endl;
	}
	m_shieldSprite.setTexture(m_shieldTexture,true);
	m_shieldSprite.setOrigin(sf::Vector2f{ 32.0f, 32.0f });
	m_shieldSprite.setPosition(m_location - MyVector2D{ 32, 32 });
}


Enemy::~Enemy()
{
}

void Enemy::render(sf::RenderWindow & t_window)
{
 	if (m_active)
	{
		t_window.draw(m_shipSprite);
#ifdef _DEBUG
		sf::CircleShape dot{ 2.0f };
		dot.setPosition(m_location);
		t_window.draw(dot);
#endif

		if (m_sheildOn)
		{
			renderShield(t_window);
		}
	}	
}

void Enemy::reset()
{
	m_active = true;
	m_location = MyVector2D{ 100.0,100.0 };
	m_velocity = MyVector2D{ 0.0,0.0 };
	m_sheildOn = 0;
	m_sheildEnergy = 200;
	m_accelarationRate = 1;
	m_enginePowerOn = false;

}

void Enemy::accelerate()
{
	float headingRadians = m_heading * PI_F / 180.0f;
	MyVector2D trust{ cos(headingRadians),sin(headingRadians) };

	trust.normalise();
	trust = trust * m_accelarationRate;
	m_velocity += trust;
}

bool Enemy::update(sf::Time t_delta)
{
	MyVector2D trust{};
	float headingRadians{ 0.0f };
	bool fireBullet{ false };
	if (m_active)
	{
		if (--m_AiTimer > 0)
		{
			switch (m_engineState)
			{
			case EngineState::Drift:
				break;
			case EngineState::Power:
				headingRadians = m_heading * PI_F / 180.0f;
				trust = MyVector2D{ cos(headingRadians),sin(headingRadians) };
				trust.normalise();
				trust = trust * m_accelarationRate;
				m_velocity += trust;				
				break;
			case EngineState::Turn:				
				m_heading += m_turnRate;
				m_shipSprite.setRotation(sf::degrees(m_heading + 90.0f));
				break;
			default:
				break;
			}
		}
		else
		{
			m_AiTimer = rand() % 100 + 100;
			double chance = rand();
			chance = chance / RAND_MAX;
			//Game1.FireBullet(location, heading, gameTime, angle, false);
			if (chance < .4)
			{
				m_engineState = EngineState::Power;
			}
			else if (chance < .7)
			{				
				m_engineState = EngineState::Drift;
			}
			else if (chance < .8)
			{
				m_engineState = EngineState::Drift;
				m_sheildOn = true;
			}
			else
			{
				m_engineState = EngineState::Turn;
			}
		}	
		if (m_AiTimer % 60 == 0)
		{
			fireBullet = true;
		}
		m_location += m_velocity;
		m_shipSprite.setPosition(m_location);
		friction();
		screenWrap();
		engineFrame();
		shield();
	}
	return fireBullet;
}

void Enemy::friction()
{
	if (m_velocity.lengthSquared() > m_maxSpeedSquared)
	{
		m_velocity = m_velocity * (m_maxSpeedSquared / m_velocity.lengthSquared());
	}
	if (m_velocity.lengthSquared() > 5.0)
	{
		m_velocity = m_velocity * FAST_FRICTION;
	}
	else
	{
		if (m_velocity.lengthSquared() > 0.1)
		{
			m_velocity = m_velocity * SLOW_FRICTION;
		}
		else
		{
			m_velocity = { 0.0,0.0 };
		}
	}
}

void Enemy::screenWrap()
{
	if (m_location.y < -32)
	{
		m_location.y = 672;
	}
	if (m_location.y > 672)
	{
		m_location.y = -32;
	}
	if (m_location.x < -32)
	{
		m_location.x = 832;
	}
	if (m_location.x > 832)
	{
		m_location.x = -32;
	}
}

void Enemy::engineFrame()
{
	if (m_enginePowerOn)
	{
		m_engineFrame += m_engineFrameIncrement;
		int frame = static_cast<int>(m_engineFrame) % m_maxEngineFrame;
		m_shipSprite.setTextureRect(sf::IntRect{ sf::Vector2i{64 * frame,0},sf::Vector2i{64,64 } });
	}
	else
	{
		m_shipSprite.setTextureRect(sf::IntRect{ sf::Vector2i{0,0},sf::Vector2i{64,64} });
	}
}

void Enemy::shield()
{
	if (m_sheildOn)
	{
		if (m_sheildEnergy-- < 0)
		{
			m_sheildOn = false;
		}
	}
	else
	{
		if (m_sheildEnergy < 300)
		{
			m_sheildEnergy++;
		}
	}
}

void Enemy::renderShield(sf::RenderWindow & t_window)
{
	m_shieldSprite.setPosition(m_shipSprite.getPosition());
	if (m_alphaUp)
	{
		m_shieldAplha += 0.1f;
	}
	else
	{
		m_shieldAplha -= 0.1f;
	}
	if (m_shieldAplha > 255.0f)
	{
		m_alphaUp = false;
	}
	if (m_shieldAplha < 50.0f)
	{
		m_alphaUp = true;
	}
	m_shieldSprite.setColor(sf::Color{ 255,255,255,static_cast<std::uint8_t>(m_shieldAplha)});
	t_window.draw(m_shieldSprite);
	m_shieldSprite.rotate(sf::degrees(0.05f));
}
