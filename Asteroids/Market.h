#ifndef MARKET
#define MARKET

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Contract.h"
const int MAX_CONTRACTS = 100;

class Market
{
public:
	Market();
	~Market();
	void render(sf::RenderWindow &t_window);
	void update(sf::Time t_deltaTime, sf::RenderWindow &t_window);
	void processEvents(const std::optional<sf::Event> t_event);
	void initialise(sf::Font & t_font);

private:
	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite{ m_backgroundTexture };
	sf::Texture m_menuIconTexture;
	sf::Sprite m_menuIconSprite{ m_menuIconTexture };
	sf::Texture m_gemsTexture;
	sf::Sprite m_gemsSprites[5] = { sf::Sprite{m_gemsTexture},sf::Sprite{m_gemsTexture},sf::Sprite{m_gemsTexture},sf::Sprite{m_gemsTexture},sf::Sprite{m_gemsTexture} };
	sf::Font m_font;	
	sf::Text m_titleText{ m_font };
	sf::Text m_valueText{ m_font };
	sf::Text m_holdingText{ m_font };
	
	sf::SoundBuffer m_completeSoundBuffer;
	sf::Sound m_completeSound{ m_completeSoundBuffer };
	int m_marketSelection = -1;

	Contract m_contracts[MAX_CONTRACTS];

	void  setupText(sf::Text &t_text, std::string t_string, sf::Vector2f t_position);
	void setupContracts(); 
};

#endif // !MARKET