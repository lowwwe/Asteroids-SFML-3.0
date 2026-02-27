#ifndef HELP
#define HELP

#include <SFML\Graphics.hpp>
// @author Peter Lowe

class Help
{
public:
	Help();
	~Help();
	void render(sf::RenderWindow &t_window);
	void update(sf::Time t_deltaTime, sf::RenderWindow &t_window);
	void processEvents(const std::optional<sf::Event> t_event);
	void initialise(sf::Font & t_font);

private:
	void setupText(sf::Text &t_text, std::string t_string, sf::Vector2f t_position);
	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite{ m_backgroundTexture };
	sf::Texture m_menuIconTexture;
	sf::Sprite m_menuIconSprite{ m_menuIconTexture };
	sf::Font m_font;
	int m_curretnPage = 0;
	sf::Text m_titleText{ m_font };
	sf::Text m_footerText[5] = { sf::Text{m_font},sf::Text{m_font},sf::Text{m_font},sf::Text{m_font},sf::Text{m_font} };
	sf::Text m_page1Text{ m_font };
	sf::Text m_page2Text{ m_font };
	sf::Text m_page3Text{ m_font };
	sf::Text m_page4Text{ m_font };
	int m_nextHelpPage = -1;
	
};

#endif // !HELP