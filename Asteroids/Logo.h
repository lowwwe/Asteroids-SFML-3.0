#ifndef LOGO
#define LOGO

#include <SFML\Graphics.hpp>

class Logo
{
public:
	Logo();
	~Logo();
	void render(sf::RenderWindow &window);
	void update(sf::Time deltaTime);
	void processEvents();
	void initialise(sf::Font & font);
private:
	sf::Time m_delayTimer; // timer to delay display
	sf::Font m_font;
	sf::Text m_liceince{ m_font };
};
#endif // LOGO
