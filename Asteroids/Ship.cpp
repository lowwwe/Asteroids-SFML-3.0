#include "Ship.h"
#include "Bullet.h"
#include <iostream>



int Ship::s_currentLevels[4] = { 0,0,0,0 };

Ship::Ship()
{
	if (!m_shipTextures[0].loadFromFile("assets\\images\\smallplayerstrip064.png"))
	{
		std::cout << "problem loading small ship 0" << std::endl;
	}
	if (!m_shipTextures[1].loadFromFile("assets\\images\\smallplayerstrip164.png"))
	{
		std::cout << "problem loading small ship 1" << std::endl;
	}
	m_shipSprite.setTexture(m_shipTextures[0]);
	m_shipSprite.setTextureRect(sf::IntRect{ sf::Vector2i{ 0,0},sf::Vector2i{64,64} });
	m_shipSprite.setPosition(m_location - MyVector2D{32, 32});
	m_shipSprite.setOrigin(sf::Vector2f{ 32.0f, 32.0f });
	m_shipSprite.rotate(sf::degrees(90.0f));
	if (!m_shieldTexture.loadFromFile("assets\\images\\shield.png"))
	{
		std::cout << "problem loading shield" << std::endl;
	}
	m_shieldSprite.setTexture(m_shieldTexture,true);
	m_shieldSprite.setOrigin(sf::Vector2f{ 32.0f, 32.0f });
	m_shieldSprite.setPosition(m_location - MyVector2D{ 32, 32 });
	if (!m_gemsTexture.loadFromFile("assets\\images\\gems.png"))
	{
		std::cout << "problem loading gems" << std::endl;
	}
	m_gemsSprite.setTexture(m_gemsTexture,true);
	if (!m_holdTexture.loadFromFile("assets\\images\\hold.png"))
	{
		std::cout << "problem loading hold" << std::endl;
	}
	m_holdSprite.setTexture(m_holdTexture);	
	for (int i = 0; i < MAX_HOLD_ITEMS; i++)
	{
		m_hold[i] = -1;
	}

	if (!m_gaugeTexture.loadFromFile("assets\\images\\gauge.png"))
	{
		std::cout << "problem loading gauge" << std::endl;
	}
	m_gaugeSprite.setTexture(m_gaugeTexture,true);
	m_gaugeSprite.setPosition(sf::Vector2f{ 620.0f, 480.0f });
	if (!m_needleTexture.loadFromFile("assets\\images\\needle.png"))
	{
		std::cout << "problem loading needle" << std::endl;
	}
	m_needleSprite.setTexture(m_needleTexture,true);
	m_needleSprite.setPosition(sf::Vector2f{ 699.0f, 560.0f });
	m_needleSprite.setOrigin(sf::Vector2f{ 62.0f, 36.0f });

#ifdef _DEBUG
	Ship::s_currentLevels[0] = 0;
	Ship::s_currentLevels[1] = 0;
	Ship::s_currentLevels[2] = 0;
	Ship::s_currentLevels[3] = 0;
#endif // _DEBUG


}


Ship::~Ship()
{
}

void Ship::render(sf::RenderWindow & t_window)
{
	if (m_active)
	{
		t_window.draw(m_shipSprite);
#ifdef _DEBUG
		sf::CircleShape dot{ 2.0f };
		dot.setPosition(m_location);
		t_window.draw(dot);
		dot.setRadius(32);
		dot.setPosition(m_location - sf::Vector2f{ 32.0f,32.0f });
		dot.setFillColor(sf::Color::Transparent);
		dot.setOutlineColor(m_warning);
		dot.setOutlineThickness(1.0f);
		t_window.draw(dot);
#endif

		if (m_sheildOn)
		{
			renderShield(t_window);
		}
	}
	renderHold(t_window);
	renderGauge(t_window);
}

void Ship::reset()
{
	m_active = true;
	m_location = MyVector2D{ 400.0 ,300.0 };
	m_accelarationRate = m_levels[ENGINE][s_currentLevels[ENGINE]];
	m_maxSpeed = static_cast<float>(5 + m_levels[ENGINE][s_currentLevels[ENGINE]]);
	m_maxSpeedSquared = m_maxSpeed * m_maxSpeed;
	m_holdCapicity = m_levels[HOLD][s_currentLevels[HOLD]];
	m_turnRate = static_cast<float>(m_levels[HOLD][s_currentLevels[HOLD]]) / 2.0f;
	m_sheildEnergy = 300 * m_levels[REACTOR][s_currentLevels[REACTOR]];
	m_velocity = MyVector2D{ 0.0,0.0 };
	m_reloading = false;
	m_reloadTime = m_levels[LASER][s_currentLevels[LASER]];
	Bullet::s_bulletLife = 200 - s_currentLevels[LASER] * 30;
	m_reloadDelay = m_reloadTime;
	m_heading = 0.0f;
	m_enginePowerOn = false;
	m_shipSprite.setRotation(sf::degrees(m_heading + 90.0f));
	for (int i = 0; i < m_holdCapicity; i++)
	{
		m_hold[i] = -1;
		 
	}
	m_guageAngle = 240.0f;
	m_guageAngleDelta = 240 / static_cast<float>(m_sheildEnergy);
	m_hyperJump = true;
	m_hyperJumpTime = 60;
 
}

void Ship::turnLeft()
{
	m_heading -= m_turnRate;
	m_shipSprite.setRotation(sf::degrees(m_heading +90.0f));
}

void Ship::turnRight()
{
	m_heading += m_turnRate;
	m_shipSprite.setRotation(sf::degrees(m_heading + 90.0f));
}

void Ship::accelerate()
{
	float headingRadians = m_heading * PI_F / 180.0f;
	MyVector2D trust{ cos(headingRadians),sin(headingRadians) };

	trust.normalise();
	trust = trust *  (m_accelarationRate*m_accelarationRate ) / 2.0f;
	m_velocity += trust;
	m_guageAngle -= m_guageAngleDelta;
	m_needleSprite.setRotation(sf::degrees(m_guageAngle));
	
}

void Ship::update(sf::Time t_delta)
{
	m_location += m_velocity;
	m_shipSprite.setPosition(m_location);
	friction();
	screenWrap();
	engineFrame();
	
	shield();
	if (m_reloadDelay-- < 0)
	{
		m_reloading = false;
		m_reloadDelay = m_reloadTime;
	}
}

void Ship::friction()
{	
	if (m_velocity.lengthSquared() > m_maxSpeedSquared*2)
	{
		m_velocity = m_velocity * ((m_maxSpeedSquared*2) / m_velocity.lengthSquared());
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

void Ship::screenWrap()
{
	if (m_location.y < -32)
	{
		m_location.y = 672;
	}
	if (m_location.y  > 672)
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

void Ship::engineFrame()
{
	if (m_enginePowerOn)
	{
		m_engineFrame += m_engineFrameIncrement;
		int frame = static_cast<int>(m_engineFrame) % m_maxEngineFrame;
		m_shipSprite.setTextureRect(sf::IntRect{ sf::Vector2i{64 * frame,0},sf::Vector2i{64,64} });
	}
	else
	{
		m_shipSprite.setTextureRect(sf::IntRect{ sf::Vector2i{ 0,0},sf::Vector2i{64,64} });
	}
}

void Ship::shield()
{
	if (m_hyperJump)
	{
		m_sheildOn = true;
	}
	if (m_sheildOn)
	{
		if (!m_hyperJump)
		{
			if (m_sheildEnergy-- < 0)
			{
				m_sheildOn = false;
			}
			else
			{
				m_guageAngle -= m_guageAngleDelta;
			}
			
		}
		else
		{
			if (m_hyperJumpTime-- < 0)
			{
				m_hyperJump = false;
				m_sheildOn = false;
			}			
		}
	}
	m_needleSprite.setRotation(sf::degrees(m_guageAngle));
}

void Ship::renderShield(sf::RenderWindow & t_window)
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
	m_shieldSprite.setColor(sf::Color{ 255,255,255,static_cast<std::uint8_t>(m_shieldAplha) });
	t_window.draw(m_shieldSprite);
	m_shieldSprite.rotate(sf::degrees(0.05f));

}

void Ship::renderHold(sf::RenderWindow & t_window)
{
	float holdOffsetX = 60.0f;
	float holdOffsetY = 540.0f;
	sf::IntRect textureRect{ sf::Vector2i{0,0},sf::Vector2i{32,32} };
	
	for (int i = 0; i < m_holdCapicity; i++)
	{
		m_holdSprite.setPosition(sf::Vector2f{ holdOffsetX, holdOffsetY });
		holdOffsetX += 4.0f;
		t_window.draw(m_holdSprite);
		if (m_hold[i] != -1)
		{
			textureRect.position.x = m_hold[i] * 32;
			m_gemsSprite.setTextureRect(textureRect);			
			m_gemsSprite.setPosition(sf::Vector2f{ holdOffsetX, holdOffsetY + 4.0f });
			t_window.draw(m_gemsSprite);
		}
		holdOffsetX += 36.0f;
	}
}

void Ship::renderGauge(sf::RenderWindow & t_window)
{
	std::uint8_t red = 240u - static_cast<std::uint8_t>(m_guageAngle);
	std::uint8_t green = static_cast<std::uint8_t>(m_guageAngle);
	std::uint8_t blue = 0;
	std::uint8_t alpha = 255;
	m_gaugeSprite.setColor(sf::Color(red, green, blue, alpha));
	t_window.draw(m_gaugeSprite);
	t_window.draw(m_needleSprite);
}

void Ship::addToHold(int t_type)
{
	for (int i = 0; i < m_holdCapicity; i++)
	{
		if (m_hold[i] == -1)
		{
			m_hold[i] = t_type;
			return;	
		}
	}
	return;	
}

int Ship::getHoldItem(int t_index)
{
	if (t_index < m_holdCapicity)
	{
		return m_hold[t_index];
	}
	return -1;
}
