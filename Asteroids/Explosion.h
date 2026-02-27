#pragma once
#include <SFML\Graphics.hpp>

const int EXPLOSION_LAST_FRAME = 99;
enum class 
	animation {
	Dust,
	Explosion
};

class Explosion
{
public:
	Explosion();
	~Explosion();
	void render(sf::RenderWindow &t_window);
	void update(sf::Time t_deltaTime);
	static void loadContent();
	void activate(sf::Vector2f t_location, animation t_type);

	static sf::Texture s_explosionTexture;
	static sf::Texture s_dustTexture;
	sf::Sprite m_explosionSprite{ s_explosionTexture };
	int m_frame;
	int m_frameSize;
	bool m_active = false;

};

