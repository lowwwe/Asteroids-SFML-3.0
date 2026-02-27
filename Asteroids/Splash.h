#ifndef SPLASH
#define SPLASH
#include <SFML\Graphics.hpp>

class Splash
{
public:
	Splash();
	~Splash();
	void render(sf::RenderWindow &window);
	void update(sf::Time deltaTime);
	void processEvents(const std::optional<sf::Event> event);
private:
	sf::Time m_delayTimer; // timer to delay display
	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite{ m_backgroundTexture };
};

#endif // !SPLASH