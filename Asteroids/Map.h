#ifndef MAP
#define MAP
#include <SFML\Graphics.hpp>




class Map
{
public:
	Map();
	~Map();
	void render(sf::RenderWindow &window);
	void update(sf::Time deltaTime, sf::RenderWindow &window);
	void processEvents(const std::optional<sf::Event> t_event);
	void initialise(sf::Font & font);
private:
	static const int maxPlanets = 9;
	//Planet pete{ static_cast<std::string>("Moon"), 0.0, 0.1, 0.1, 0.0, 0.0, 0.9, sf::IntRect{ 534,41,30,25 } };
	/*Planet m_planets[maxPlanets]{
		{ static_cast<std::string>("Moon"),		 0.0, 0.1, 0.1, 0.0, 0.0, 0.9, sf::IntRect{ 534,41,30,25 } },
		{ static_cast<std::string>("Venus"),	 0.2, 0.2, 0.0, 0.0, 0.0, 0.1, sf::IntRect{ 250, 10, 30, 50 }},
	{ static_cast<std::string>("Mercury"),		 0.5, 0.1, 0.0, 0.0, 0.0, 0.5, sf::IntRect{ 280, 20, 80, 80 } },
	{ static_cast<std::string>("Earth"),		 0.15, 0.15, 0.15, 0.4, 0.0, 0.2, sf::IntRect{ 370, 45, 120, 90 } },

	{ static_cast<std::string>("Mars"),		 0.1, 0.8, 0.0, 0.0, 0.0, 0.3, sf::IntRect{ 506, 130, 100, 60 } },
	{ static_cast<std::string>("Jupiter"),		 0.1, 0.1, 0.1, 0.1, 0.1, 0.4, sf::IntRect{ 530, 240, 200, 200 } },
	{ static_cast<std::string>("Saturn"),		 0.2, 0.2, 0.2, 0.2, 0.2, 0.8, sf::IntRect{ 406, 331, 150, 120 } },
	{ static_cast<std::string>("neptune"),		 0.1, 0.0, 0.0, 0.4, 0.5, 0.9, sf::IntRect{ 360, 427, 130, 121 } },
	{ static_cast<std::string>("uranus"),		 0.0, 0.0, 0.0, 0.2, 0.5, 0.4, sf::IntRect{ 133, 400, 200, 200 } },
	};
	*/
	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite{ m_backgroundTexture };
	sf::Texture m_planetTextures[maxPlanets];
	sf::Sprite m_planetSprites[maxPlanets] = { 
		sf::Sprite{m_planetTextures[0]},
		sf::Sprite{m_planetTextures[1]},
		sf::Sprite{m_planetTextures[2]},
		sf::Sprite{m_planetTextures[3]},
		sf::Sprite{m_planetTextures[4]},
		sf::Sprite{m_planetTextures[5]},
		sf::Sprite{m_planetTextures[6]},
		sf::Sprite{m_planetTextures[7]},
		sf::Sprite{m_planetTextures[9]}
		};
	sf::Texture m_gemsTexture;
	sf::Sprite m_gemsSprite{ m_gemsTexture };
	sf::Texture m_menuTexture;
	sf::Sprite m_menuSprite{ m_menuTexture };
	sf::Font m_font;
	sf::Text m_planetName{ m_font };
	sf::Text m_pirates{ m_font };
	sf::Text m_gemProbability{ m_font };


	bool m_mouseClick;
	
};

#endif // !MAP