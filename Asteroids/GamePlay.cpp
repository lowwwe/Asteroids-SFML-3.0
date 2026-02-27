#include "GamePlay.h"
#include "Game.h"
#include <iostream>




GamePlay::GamePlay()
{
	if (!m_laserSoundBuffer.loadFromFile("assets\\audio\\laser.wav"))
	{
		std::cout << "problem loading laser sound" << std::endl;
	}
	m_laserSound.setBuffer(m_laserSoundBuffer);
	if (!m_enemyLaserSoundBuffer.loadFromFile("assets\\audio\\enemy laser.wav"))
	{
		std::cout << "problem loading enemy laser sound" << std::endl;
	}
	m_enemyLaserSound.setBuffer(m_enemyLaserSoundBuffer);
	if (!m_explosionSoundBuffer.loadFromFile("assets\\audio\\explosionSound.wav"))
	{
		std::cout << "problem loading explosion sound" << std::endl;
	}
	m_explosionSound.setBuffer(m_explosionSoundBuffer);	
	if (!m_engineSoundBuffer.loadFromFile("assets\\audio\\engine.wav"))
	{
		std::cout << "problem loading engine sound" << std::endl;
	}
	m_engineSound.setBuffer(m_engineSoundBuffer);
	m_engineSound.setLooping(true);
	m_engineSound.stop();
	if (!m_asteroidBreakSoundBuffer.loadFromFile("assets\\audio\\Asteroid breaking.wav"))
	{
		std::cout << "problem loading asteroid sound" << std::endl;
	}
	m_asteroidBreakSound.setBuffer(m_asteroidBreakSoundBuffer);
	
	if (!Bullet::s_bulletTexture.loadFromFile("assets\\images\\bullet16.png"))
	{
		std::cout << "problem loading bullet texture" << std::endl;
	}
	m_explosions[0].loadContent();
	m_crystals[0].loadContent();
}


GamePlay::~GamePlay()
{
}

void GamePlay::render(sf::RenderWindow & t_window)
{
	//t_window.draw(m_fuelLeft);
	m_ship.render(t_window);
	m_PirateShip.render(t_window);
	for (size_t i = 0; i < MAX_ASTEROIDS; i++)
	{
		m_asteroids[i].render(t_window);
	}
	for (size_t i = 0; i < MAX_BULLETS; i++)
	{
		m_bullets[i].render(t_window);
	}
	for (int i = 0; i < MAX_CRYSTALS; i++)
	{
		m_crystals[i].render(t_window);
	}
	for (int i = 0; i < MAX_EXPLOSIONS; i++)
	{
		m_explosions[i].render(t_window);
	}
	

	
}

void GamePlay::pauseRender(sf::RenderWindow & t_window)
{
	render(t_window);
	t_window.draw(m_pausePromptText);
	t_window.draw(m_resumeText);
	t_window.draw(m_returnToBaseText);
}

void GamePlay::gameoverRender(sf::RenderWindow & t_window)
{
	render(t_window);
	t_window.draw(m_gameOverText);	
}

void GamePlay::update(sf::Time t_deltaTime)
{
	if (m_shipTrunRight)
	{
		m_ship.turnRight();
	}
	if (m_shipTrunLeft)
	{
		m_ship.turnLeft();
	}
	if (m_shipAccelerate && m_ship.m_sheildEnergy > 0)
	{
		m_ship.accelerate();	
		m_ship.m_sheildEnergy--;
		m_ship.m_enginePowerOn = true;
	}
	if (m_fire & !m_ship.m_reloading)
	{
		fireBullet(true);
		m_fire = false;
		m_ship.m_reloading = true;
	}
	m_ship.update(t_deltaTime);
	m_fuelLeft.setString(std::to_string(m_ship.m_sheildEnergy));
	if (m_PirateShip.update(t_deltaTime))
	{
		fireBullet(false);
	}
	for (size_t i = 0; i < MAX_ASTEROIDS; i++)
	{
		m_asteroids[i].update(t_deltaTime);
	}
	for (size_t i = 0; i < MAX_BULLETS; i++)
	{
		m_bullets[i].update(t_deltaTime);
	}
	for (int i = 0; i < MAX_EXPLOSIONS; i++)
	{
		m_explosions[i].update(t_deltaTime);
	}
	for (int i = 0; i < MAX_CRYSTALS; i++)
	{
		m_crystals[i].update(t_deltaTime);
	}
	collisions();
}

void GamePlay::pauseUpdate(sf::Time t_deltaTime)
{
}

void GamePlay::overUpdate(sf::Time t_deltaTime)
{
	for (size_t i = 0; i < MAX_ASTEROIDS; i++)
	{
		m_asteroids[i].update(t_deltaTime);
	}
	for (size_t i = 0; i < MAX_BULLETS; i++)
	{
		m_bullets[i].update(t_deltaTime);
	}
	for (int i = 0; i < MAX_EXPLOSIONS; i++)
	{
		m_explosions[i].update(t_deltaTime);
	}
	for (int i = 0; i < MAX_CRYSTALS; i++)
	{
		m_crystals[i].update(t_deltaTime);
	}
	if (m_gameOverCounter-- < 0)
	{
		Game::s_currentGameState = GameState::Hub;
	}
	m_shipAccelerate = false;
	m_engineSound.stop();
	m_ship.m_enginePowerOn = false;
	m_shipTrunLeft = false;
	m_shipTrunRight = false;
}

void GamePlay::processEvents(const std::optional<sf::Event> t_event)
{
	if (t_event->is<sf::Event::KeyPressed>()) //user pressed a key
	{
		const sf::Event::KeyPressed* newKeypress = t_event->getIf<sf::Event::KeyPressed>();

		if (sf::Keyboard::Key::Right == newKeypress->code || sf::Keyboard::Key::D == newKeypress->code)
		{
			m_shipTrunRight = true;
			m_shipTrunLeft = false;
		}
		if (sf::Keyboard::Key::Left == newKeypress->code || sf::Keyboard::Key::A == newKeypress->code)
		{
			m_shipTrunLeft = true;
			m_shipTrunRight = false;
		}
		if (sf::Keyboard::Key::Up == newKeypress->code || sf::Keyboard::Key::W == newKeypress->code)
		{
			if (!m_shipAccelerate && m_ship.m_sheildEnergy > 0)
			{
				m_shipAccelerate = true;
				m_engineSound.play();
				std::cout << "play engine" << std::endl;
			}
		}
		if (sf::Keyboard::Key::LControl == newKeypress->code || sf::Keyboard::Key::RControl == newKeypress->code)
		{
			m_fire = true; 			
		}
		if (sf::Keyboard::Key::LShift == newKeypress->code || sf::Keyboard::Key::RShift == newKeypress->code)
		{
			m_ship.m_sheildOn = true;
		}
		if (sf::Keyboard::Key::LAlt == newKeypress->code || sf::Keyboard::Key::RAlt == newKeypress->code)
		{
			if (m_ship.m_sheildEnergy > 100 && !m_ship.m_hyperJump )
			{
				m_ship.m_sheildOn = true;
				m_ship.m_hyperJump = true;
				m_ship.m_hyperJumpTime = 60;
				m_ship.m_sheildEnergy -= 100;
				m_ship.m_location = MyVector2D{ static_cast<double>(std::rand() % 800),static_cast<double>(std::rand() % 600 )};
				m_ship.m_guageAngle -= m_ship.m_guageAngleDelta * 100.0f;
			}
		}
	}
	if (t_event->is<sf::Event::KeyReleased>()) //user pressed a key
	{
		const sf::Event::KeyReleased* newKeyrelease = t_event->getIf<sf::Event::KeyReleased>();
		if (sf::Keyboard::Key::Right == newKeyrelease->code || sf::Keyboard::Key::D == newKeyrelease->code)
		{
			m_shipTrunRight = false;
		}
		if (sf::Keyboard::Key::Left == newKeyrelease->code || sf::Keyboard::Key::A == newKeyrelease->code)
		{
			m_shipTrunLeft = false;
		
		}
		if (sf::Keyboard::Key::Up == newKeyrelease->code || sf::Keyboard::Key::W == newKeyrelease->code)
		{
			m_shipAccelerate = false;
			m_engineSound.stop();
			m_ship.m_enginePowerOn = false;
		}
		if (sf::Keyboard::Key::LShift == newKeyrelease->code || sf::Keyboard::Key::RShift == newKeyrelease->code)
		{
			m_ship.m_sheildOn = false;
		}
		if (sf::Keyboard::Key::Escape == newKeyrelease->code)
		{
			Game::s_currentGameState = GameState::Pause;
		}
	}
}

void GamePlay::pauseProcessEvents(const std::optional<sf::Event> t_event)
{
	if (t_event->is<sf::Event::MouseMoved>())
	{
		const sf::Event::MouseMoved* newMouseMove = t_event->getIf<sf::Event::MouseMoved>();

	
	
		m_pauseOption = PauseOption::None;
	//	m_resumeText.setFillColor(sf::Color::White);
//		m_returnToBaseText.setFillColor(sf::Color::White);
		if (newMouseMove->position.x > 300 && newMouseMove->position.x < 500)
		{
			if (newMouseMove->position.y > 250 && newMouseMove->position.y < 300)
			{
				m_pauseOption = PauseOption::Resume;
				m_resumeText.setFillColor(sf::Color::Yellow);
				m_returnToBaseText.setFillColor(sf::Color::White);
				
			}
			if (newMouseMove->position.y > 300 && newMouseMove->position.y < 350)
			{
				m_pauseOption = PauseOption::Base;
				m_returnToBaseText.setFillColor(sf::Color::Yellow);
				m_resumeText.setFillColor(sf::Color::White);
			}			
		}
	}
	if (t_event->is<sf::Event::MouseButtonReleased>())
	{	
	
		if (m_pauseOption == PauseOption::Resume)
		{
			Game::s_currentGameState = GameState::Game;
			m_resumeText.setFillColor(sf::Color::White);
			m_returnToBaseText.setFillColor(sf::Color::White);
			m_pauseOption = PauseOption::None;
		}
		if (m_pauseOption == PauseOption::Base)
		{
			retrieveCargo();
			Game::s_currentGameState = GameState::Hub;
			Game::s_music = Music::Menu;
			m_resumeText.setFillColor(sf::Color::White);
			m_returnToBaseText.setFillColor(sf::Color::White);
			m_pauseOption = PauseOption::None;
		}
	}
	if (t_event->is<sf::Event::KeyReleased>()) //user pressed a key
	{
		const sf::Event::KeyReleased* newKeyrelease = t_event->getIf<sf::Event::KeyReleased>();
		if (sf::Keyboard::Key::Escape == newKeyrelease->code)
		{
			Game::s_currentGameState = GameState::Game;
		}
	}
	if (t_event->is<sf::Event::KeyPressed>()) //user pressed a key
	{
		const sf::Event::KeyPressed* newKeypress = t_event->getIf<sf::Event::KeyPressed>();
		if (sf::Keyboard::Key::Up == newKeypress->code)
		{
			if (m_pauseOption == PauseOption::Base)
			{
				m_pauseOption =PauseOption::Resume;
				m_resumeText.setFillColor(sf::Color::Yellow);
				m_returnToBaseText.setFillColor(sf::Color::White);
			}
			else
			{
				m_pauseOption = PauseOption::Base;
				m_returnToBaseText.setFillColor(sf::Color::Yellow);
				m_resumeText.setFillColor(sf::Color::White);
			}
		}
		if (sf::Keyboard::Key::Down == newKeypress->code)
		{
			if (m_pauseOption == PauseOption::Resume)
			{
				m_pauseOption = PauseOption::Base;
				m_returnToBaseText.setFillColor(sf::Color::Yellow);
				m_resumeText.setFillColor(sf::Color::White);
			}
			else
			{
				m_pauseOption =PauseOption::Resume;
				m_resumeText.setFillColor(sf::Color::Yellow);
				m_returnToBaseText.setFillColor(sf::Color::White);
			}
		}
		if (sf::Keyboard::Key::Enter == newKeypress->code)
		{
			if (m_pauseOption == PauseOption::Resume)
			{
				Game::s_currentGameState = GameState::Game;
				m_returnToBaseText.setFillColor(sf::Color::White);
				m_resumeText.setFillColor(sf::Color::White);
				m_pauseOption = PauseOption::None;

			}
			if (m_pauseOption == PauseOption::Base)
			{
				retrieveCargo();
				Game::s_currentGameState = GameState::Hub;
				Game::s_music = Music::Menu;
				m_resumeText.setFillColor(sf::Color::White);
				m_returnToBaseText.setFillColor(sf::Color::White);
				m_pauseOption = PauseOption::None;
			}
		}
	}
}

void GamePlay::initialise(sf::Font & t_font)
{
	m_font = t_font;
	setupText(m_pausePromptText, "Game Paused", sf::Vector2f{ 280.0f, 200.0f });
	m_pausePromptText.setCharacterSize(32);
	setupText(m_resumeText, "Resume Game", sf::Vector2f{ 300.0f, 250.0f });
	setupText(m_returnToBaseText, "Return to Base", sf::Vector2f{ 300.0f, 300.0f });	
	setupText(m_gameOverText, "You Died !", sf::Vector2f{ 300.0f, 300.0f });
	setupText(m_fuelLeft, "300", sf::Vector2f{ 675.0f,570.0f });
}

void GamePlay::setupLevel(int t_levelNo)
{
	m_ship.reset();
	m_fuelLeft.setString(std::to_string(m_ship.m_sheildEnergy));
	for (size_t i = 0; i < MAX_ASTEROIDS; i++)
	{
		m_asteroids[i].m_active = false;
	}
	int asteroidCount;
	asteroidCount = (t_levelNo < 2) ? 2 : t_levelNo;
	for (int i = 0; i < asteroidCount; i++)
	{
		m_asteroids[i].reStart(i);
	}
	for (int i = 0; i < MAX_CRYSTALS; i++)
	{
		m_crystals[i].m_active = false;
	}
	m_currentLevel = t_levelNo;
	m_gameOverCounter = 300;
	// add pirates
	double chance = rand()/ RAND_MAX;
	if (chance < Game::g_planets[Game::s_currentPlanet].pirates)
	{
		m_PirateShip.reset();		
	}
	else
	{
		m_PirateShip.m_active = false;
	}
	// do crystals
	// do explosions
}

void GamePlay::fireBullet(bool t_friendly)
{
	int i{ 0 };
	bool found{ false };
	float heading{0.0};
	MyVector2D location{ 0.0,0.0 };

	if (t_friendly)
	{
		heading = m_ship.m_heading;
		location = m_ship.m_location;
	}
	else
	{
		heading = m_PirateShip.m_heading;
		location = m_PirateShip.m_location;
	}
	while( !found && i < MAX_BULLETS)
	{
		if (!m_bullets[i].m_alive)
		{
			found = true;
			float xBit = std::cos(heading /180.0f * PI_F);
			float yBit = std::sin(heading / 180.0f * PI_F);
			m_bullets[i].reStart(1, location, MyVector2D{ xBit,yBit }, heading, t_friendly);
			m_laserSound.play();
		}
		++i;
	}
}

void GamePlay::collisions()
{
#ifdef _DEBUG
	m_ship.m_warning = sf::Color::Transparent;
#endif // _DEBUG
		
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (m_bullets[i].m_alive)
		{
			if (m_bullets[i].m_friendly )
			{
				if (m_PirateShip.m_active)
				{
					if (checkBulletShip(m_PirateShip.m_location, m_bullets[i]))
					{
						m_bullets[i].m_alive = false;
						damagePirateShip();
					}
				}
			}
			else
			{
				if (checkBulletShip(m_ship.m_location, m_bullets[i]))
				{
					m_bullets[i].m_alive = false;
					damageShip();
				}
			}

			for (int j = 0; j < MAX_ASTEROIDS; j++)
			{
				if (m_asteroids[j].m_active)
				{
					if (checkBulletAsteroid(m_bullets[i], m_asteroids[j]))
					{
						m_bullets[i].m_alive = false;
						bool found = false;
						int index = 0;
						while (!found && index < MAX_ASTEROIDS)
						{
							if(!m_asteroids[index].m_active)
							{ 
								found = true;
							}
							else
							{
								index++;
							}
						}
						if (found)
						{
							if (m_asteroids[j].reSize(m_bullets[i], m_asteroids[index]))
							{								
								newExplosion(m_asteroids[j].m_location, animation::Dust);
								newCrystal(m_asteroids[j].m_location, -1);
							}
							else
							{
								m_asteroidBreakSound.play();								
							}
						}
						else
						{
							m_asteroids[j].destroy();													
							newCrystal(m_asteroids[j].m_location, -1);
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < MAX_CRYSTALS; i++)
	{
		if (m_crystals[i].m_active)
		{
			if (checkShipCrystal(m_ship.m_location, m_crystals[i].m_location))
			{
				m_crystals[i].m_active = false;
				m_ship.addToHold(m_crystals[i].m_type);
			}
		}
	}
	for (int j = 0; j < MAX_ASTEROIDS; j++)
	{
		if (m_asteroids[j].m_active)
		{
			if (checkShipAsteroid(m_ship.m_location, m_asteroids[j]))
			{
				destroyAsteroid(m_asteroids[j]);
				damageShip();
			}
		}
	}
	if (checkShipPirate())
	{
		damageShip();
		damagePirateShip();
	}
}

bool GamePlay::checkBulletShip(MyVector2D t_location,Bullet & t_bullet)
{
	float gap = 64.0f + 16.0f;
	
	if (t_bullet.m_location.y < t_location.y - gap)
	{
		return false;
	}
	if (t_bullet.m_location.y > t_location.y + gap)
	{
		return false;
	}
	if (t_bullet.m_location.x < t_location.x - gap)
	{
		return false;
	}
	if (t_bullet.m_location.x > t_location.x + gap)
	{
		return false;
	}
#ifdef _DEBUG
	m_ship.m_warning = sf::Color::Magenta;
#endif // _DEBUG
	if ((t_location - t_bullet.m_location).lengthSquared() > (gap * gap / 4))
	{
		return false;
	}
	return true;
}

bool GamePlay::checkShipPirate()
{
	float gap = 128.0f;
	if (!m_PirateShip.m_active)
	{
		return false;
	}
	if (m_ship.m_location.y < m_PirateShip.m_location.y - gap)
	{
		return false;
	}
	if (m_ship.m_location.y > m_PirateShip.m_location.y + gap)
	{
		return false;
	}
	if (m_ship.m_location.x < m_PirateShip.m_location.x - gap)
	{
		return false;
	}
	if (m_ship.m_location.x > m_PirateShip.m_location.x + gap)
	{
		return false;
	}
#ifdef _DEBUG
	m_ship.m_warning = sf::Color::Magenta;
#endif // _DEBUG
	if ((m_PirateShip.m_location - m_ship.m_location).lengthSquared() > (gap * gap / 4))
	{
		return false;
	}
	return true;	
}

void GamePlay::damagePirateShip()
{
	if (!m_PirateShip.m_sheildOn)
	{
		
		m_PirateShip.m_active = false;
		newExplosion(m_PirateShip.m_location, animation::Explosion);
		newCrystal(m_PirateShip.m_location, 5);
		m_explosionSound.play();
	}
}

void GamePlay::destroyAsteroid(Asteroid &t_asteroid)
{
	bool found = false;
	int index = 0;
	while (!found && index < MAX_ASTEROIDS)
	{
		if (!m_asteroids[index].m_active)
		{
			found = true;
		}
		else
		{
			index++;
		}
	}
	if (found)
	{
		if (t_asteroid.reSize(m_ship.m_location, m_ship.m_velocity, m_asteroids[index]))
		{
			newExplosion(t_asteroid.m_location, animation::Dust);
			newCrystal(t_asteroid.m_location, -1);
		}
		else
		{
			m_asteroidBreakSound.play();
		}
	}
	else
	{
		t_asteroid.destroy();
		newCrystal(t_asteroid.m_location, -1);
	}
}

void GamePlay::damageShip()
{
	if (!m_ship.m_sheildOn)
	{
		Game::s_currentGameState = GameState::Over;
		m_ship.m_active = false;
		newExplosion(m_ship.m_location, animation::Explosion);
		m_explosionSound.play();
	}
}

bool GamePlay::checkShipAsteroid(MyVector2D t_shipLocation, Asteroid & t_asteroid)
{
	float gap = Asteroid::s_sizes[t_asteroid.m_size] + 64.0f;
	if (t_shipLocation.y < t_asteroid.m_location.y - gap)
	{
		return false;
	}
	if (t_shipLocation.y > t_asteroid.m_location.y + gap)
	{
		return false;
	}
	if (t_shipLocation.x < t_asteroid.m_location.x - gap)
	{
		return false;
	}
	if (t_shipLocation.x > t_asteroid.m_location.x + gap)
	{
		return false;
	}
	if ((t_asteroid.m_location - t_shipLocation).lengthSquared() > (gap * gap / 4))
	{
		return false;
	}
	return true;
}

bool GamePlay::checkBulletAsteroid(Bullet & t_bullet, Asteroid & t_asteroid)
{	
	float gap = Asteroid::s_sizes[ t_asteroid.m_size] + 16.0f;
	if (t_bullet.m_location.y < t_asteroid.m_location.y - gap)
	{
		return false;
	}
	if (t_bullet.m_location.y > t_asteroid.m_location.y + gap)
	{
		return false;
	}
	if (t_bullet.m_location.x < t_asteroid.m_location.x - gap)
	{
		return false;
	}
	if (t_bullet.m_location.x > t_asteroid.m_location.x + gap)
	{
		return false;
	}
	if ((t_asteroid.m_location - t_bullet.m_location).lengthSquared() > (gap * gap / 4))
	{
		return false;
	}
	return true;
}

void GamePlay::newExplosion(MyVector2D t_location, animation t_type)
{
	int i{ 0 };
	bool found{ false };
	while (!found && i < MAX_EXPLOSIONS)
	{
		if (!m_explosions[i].m_active)
		{
			found = true;
			m_explosions[i].activate(t_location , t_type);
		}
		i++;
	}

}


int GamePlay::chooseCrystal()
{
	double decimal = (rand() % 1000) / 1000.0;
	double checkpoint{ 0.0 };
	
	for (int i = 0; i < NO_MINERALS; i++)
	{
		checkpoint += Game::g_planets[Game::s_currentPlanet].minerals[i];
		if (decimal < checkpoint)
		{
			return i;
		}
	}
	return -1;
}
bool GamePlay::checkShipCrystal(MyVector2D t_shipLocation, MyVector2D t_crystalLocation)
{
	const double touchingDistance = 48.0; //  64/2 + 32/2 - 8 {8 to ensure sprites touch}
	const double touchingDistanceSquared = touchingDistance * touchingDistance;
	MyVector2D gap = t_shipLocation - t_crystalLocation;
	if (gap.x < -touchingDistance)
	{
		return false;
	}
	if (gap.x > touchingDistance)
	{
		return false;
	}
	if (gap.y < -touchingDistance)
	{
		return false;
	}
	if (gap.y > touchingDistance)
	{
		return false;
	}
#ifdef _DEBUG
	m_ship.m_warning = sf::Color::Blue;
#endif // _DEBUG
	if (gap.lengthSquared() < touchingDistanceSquared)
	{
		return false;
	}
	return true;
}
void GamePlay::setupText(sf::Text & t_text, std::string t_string, sf::Vector2f t_position)
{
	t_text.setFont(m_font);
	t_text.setCharacterSize(24);
	t_text.setString(t_string);
	t_text.setFillColor(sf::Color::White);
	t_text.setPosition(t_position);
}
void GamePlay::retrieveCargo()
{
	int index = 0;
	int gemType= -1;
	bool clearedLevel = true;
	do
	{
		gemType = m_ship.getHoldItem(index++);
		Game::s_gems[gemType]++;
	} 
	while (gemType != -1);
	
	for (int i = 0; i < m_currentLevel; i++)
	{
		if (m_asteroids[i].m_active)
		{
			clearedLevel = false;
		}
	}
	if (m_currentLevel < MAX_PLANETS -1 && clearedLevel)
	{
		Game::g_planets[m_currentLevel + 1].active = true;	
	}

}
void GamePlay::newCrystal(MyVector2D t_location, int t_type)
{
	int i{ 0 };
	bool found{ false };	
	if (t_type == -1)
	{
		t_type = chooseCrystal();
	}
	if (t_type == -1)
	{
		return;
	}
	while (!found && i <MAX_CRYSTALS)
	{
		if (!m_crystals[i].m_active)
		{
			found = true;
			m_crystals[i].activate(t_location, t_type);
		}
		i++;
	}
}
