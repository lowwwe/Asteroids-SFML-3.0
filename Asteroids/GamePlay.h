#ifndef GAMEPLAY
#define GAMEPLAY
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Ship.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Explosion.h"
#include "Crystal.h"
#include "Enemy.h"


const int MAX_ASTEROIDS = 20;
const int MAX_EXPLOSIONS = 5;
const int MAX_CRYSTALS = 10;


//const int RESUME = 1;
//const int RETURN = 2;
enum class
	PauseOption {
	Resume,
	Base,
	None
};


class GamePlay
{
public:
	GamePlay();
	~GamePlay();
	void render(sf::RenderWindow &t_window);
	void pauseRender(sf::RenderWindow &t_window);
	void gameoverRender(sf::RenderWindow &t_window);
	void update(sf::Time t_deltaTime);
	void pauseUpdate(sf::Time t_deltaTime);
	void overUpdate(sf::Time t_deltaTime);
	void processEvents(const std::optional<sf::Event> t_event);
	void pauseProcessEvents(const std::optional<sf::Event> t_event);
	void initialise(sf::Font & t_font);
	void setupLevel(int t_levelNo);
private:
	
	Ship m_ship;
	Asteroid m_asteroids[MAX_ASTEROIDS];
	Bullet m_bullets[MAX_BULLETS];
	Explosion m_explosions[MAX_EXPLOSIONS];
	Enemy m_PirateShip;
	Crystal m_crystals[MAX_CRYSTALS];
	sf::SoundBuffer m_laserSoundBuffer;  
	sf::Sound m_laserSound{ m_laserSoundBuffer };
	sf::SoundBuffer m_enemyLaserSoundBuffer;
	sf::Sound m_enemyLaserSound{ m_enemyLaserSoundBuffer };
	sf::SoundBuffer m_explosionSoundBuffer;
	sf::Sound m_explosionSound{ m_explosionSoundBuffer };
	sf::SoundBuffer m_engineSoundBuffer;
	sf::Sound m_engineSound{ m_engineSoundBuffer };
	sf::SoundBuffer m_asteroidBreakSoundBuffer;
	sf::Sound m_asteroidBreakSound{ m_asteroidBreakSoundBuffer };
	sf::Font m_font;
	sf::Text m_pausePromptText{ m_font };
	sf::Text m_resumeText{ m_font };
	sf::Text m_returnToBaseText{ m_font };
	sf::Text m_gameOverText{ m_font };
	
	sf::Text m_fuelLeft{ m_font };


	bool m_shipTrunRight{ false };
	bool m_shipTrunLeft{ false };
	bool m_shipAccelerate{ false };
	bool m_fire{ false };
	PauseOption m_pauseOption{ PauseOption::None};
	int m_currentLevel; 
	int m_gameOverCounter;

	void fireBullet(bool t_friendly);
	void collisions();
	bool checkBulletShip(MyVector2D t_location, Bullet &t_bullet);
	bool checkShipPirate();
	void damagePirateShip();
	bool checkShipAsteroid(MyVector2D t_shipLocation, Asteroid &t_asteroid);
	bool checkBulletAsteroid(Bullet &t_bullet, Asteroid &t_asteroid);
	void newExplosion(MyVector2D t_location, animation t_type);
	void newCrystal(MyVector2D t_location, int t_type);
	int chooseCrystal();
	bool checkShipCrystal(MyVector2D t_shipLocation, MyVector2D t_crystalLocation);
	void setupText(sf::Text & t_text, std::string t_string, sf::Vector2f t_position);
	void retrieveCargo();
	void destroyAsteroid(Asteroid &t_asteroid);
	void damageShip();
	
};

#endif // !GAMEPLAY