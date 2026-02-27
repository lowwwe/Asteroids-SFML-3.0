#ifndef HUB
#define HUB
#include <SFML\Graphics.hpp>

enum class
	HubRegion {
	Hanger,
	Map,
	Market,
	Help,
	Exit,
	None
};

class Hub
{
public:
	Hub();
	~Hub();
	void render(sf::RenderWindow &window);
	void update(sf::Time deltaTime, sf::RenderWindow &window);
	void processEvents(const std::optional<sf::Event> t_event);
	void initialise(sf::Font & font);
private:
	void  setupText(sf::Text &text, std::string t_string, sf::Vector2f position);

	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite{ m_backgroundTexture };
	HubRegion m_currentRegion; // currently highlighted region
	sf::Font m_font;
	sf::Text m_mapText{m_font};
	sf::Text m_hangerText{ m_font };
	sf::Text m_marketText{ m_font };
	sf::Text m_helpText{ m_font };
	sf::Text m_exitText{ m_font };
	
	
};

#endif // !HUB