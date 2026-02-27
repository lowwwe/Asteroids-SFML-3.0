#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "MyVector2D.h"
#include "constants.h"






class Enemy
{
public:
	Enemy();
	~Enemy();

	void render(sf::RenderWindow &t_window);
	bool m_active = true;
	void reset();
	void accelerate();
	bool update(sf::Time t_delta);
	void friction();
	void damage();
	MyVector2D m_location{ 400.0 ,300.0 };
	bool m_enginePowerOn{ false };
	float m_heading = 0.0f;
	bool m_sheildOn = false;
	float m_shieldAplha = 0;
	bool m_alphaUp = true;
	int m_sheildEnergy = 500;
	MyVector2D m_velocity;
	MyVector2D m_trust;
private:
	EngineState m_engineState{ EngineState::Drift};
	sf::Texture m_shipTexture;
	sf::Sprite m_shipSprite{ m_shipTexture };
	sf::Texture m_shieldTexture;
	sf::Sprite m_shieldSprite{ m_shipTexture };
	float m_turnRate = 2.0f;
	int m_accelarationRate = 20;
	int m_AiTimer = 100;

	float m_maxSpeed;
	float m_maxSpeedSquared = 25.0f;
	float m_engineFrame = 0.0f;
	float m_engineFrameIncrement = 0.5f;

	int m_maxEngineFrame = 6;
	void screenWrap();
	void engineFrame();
	void shield();
	void renderShield(sf::RenderWindow &t_window);
};

#endif // !ENEMY_H