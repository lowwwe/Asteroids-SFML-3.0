#ifndef SHIP
#define SHIP
#include <SFML\Graphics.hpp>
#include "MyVector2D.h"
#include "constants.h"

const int ENGINE = 0; // Array index for ship parameters
const int LASER = 1;
const int HOLD = 2;
const int REACTOR = 3;

const int MAX_HOLD_ITEMS = 10;

class Ship
{
public:
	Ship();
	~Ship();
	void render(sf::RenderWindow &t_window);
	bool m_active = true;
	void reset();
	void turnLeft();
	void turnRight();
	void accelerate();
	void update(sf::Time t_delta);
	void friction();
	MyVector2D m_location{ 400.0 ,300.0 };
	bool m_enginePowerOn{ false };
	float m_heading = 0.0f;
	bool m_sheildOn = false;
	float m_shieldAplha = 0;
	bool m_alphaUp = true;
	int m_sheildEnergy = 500;
	float m_guageAngle = 0.0f;
	float m_guageAngleDelta = 1.0f;
	int m_levels[4][4] = { { 1,2,3,4 },{ 20,15,10,5 },{ 3,5,7,10 },{ 1,2,3,4  } };
	static int s_currentLevels[4];
	int m_hold[MAX_HOLD_ITEMS];
	void addToHold(int t_type);
	int getHoldItem(int t_index);
	bool m_reloading;
	bool m_hyperJump = true;
	int m_hyperJumpTime = 60;
	MyVector2D m_velocity;
	sf::Color m_warning = sf::Color::Yellow;

private:
	sf::Texture m_shipTextures[2];
	sf::Sprite m_shipSprite{ m_shipTextures[0]};
	sf::Texture m_shieldTexture;
	sf::Sprite m_shieldSprite{ m_shieldTexture };
	sf::Texture m_holdTexture;
	sf::Sprite m_holdSprite{ m_holdTexture };
	sf::Texture m_gemsTexture;
	sf::Sprite m_gemsSprite{ m_gemsTexture };
	sf::Texture m_needleTexture;
	sf::Sprite m_needleSprite{ m_needleTexture };
	sf::Texture m_gaugeTexture;
	sf::Sprite m_gaugeSprite{ m_gaugeTexture };
	
	
	
	
	float m_turnRate = 2.0f;
	int m_accelarationRate = m_levels[ENGINE][s_currentLevels[ENGINE]];
	int m_reloadTime;
	
	int m_reloadDelay;
	float m_maxSpeed;
	float m_maxSpeedSquared = 25.0f;
	float m_engineFrame = 0.0f;
	float m_engineFrameIncrement = 0.5f;
	
	int m_maxEngineFrame = 6;
	int m_holdCapicity = m_levels[HOLD][s_currentLevels[HOLD]];
	int m_holdCurrentCount = 0;
	void screenWrap();
	void engineFrame();
	void shield();
	void renderShield(sf::RenderWindow &t_window);
	void renderHold(sf::RenderWindow &t_window);
	void renderGauge(sf::RenderWindow &t_window);
	

};

#endif // !SHIP