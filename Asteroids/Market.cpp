#include "Market.h"
#include "Game.h"
#include <iostream>

const float MARKET_LINE_GAP = 48.0f;
const float CONTRACT_OFFSET_X = 300.0f;
const float CONTRACT_OFFSET_Y = 200.0f;
const float VALUE_OFFSET_X = 600.0f;
const float INTER_GEM_GAP = 36.0f;

Market::Market()
{
	if (!m_backgroundTexture.loadFromFile("ASSETS\\IMAGES\\menu.jpg"))
	{
		std::cout << "problem with menu" << std::endl;
	}
	m_backgroundSprite.setTexture(m_backgroundTexture,true);
	m_backgroundSprite.setPosition(sf::Vector2f{ 0.0f, 0.0f });
	if (!m_menuIconTexture.loadFromFile("ASSETS\\IMAGES\\menuicon.png"))
	{
		std::cout << "problem with menu" << std::endl;
	}
	m_menuIconSprite.setTexture(m_menuIconTexture,true);
	m_menuIconSprite.setPosition(sf::Vector2f{ 750.0f, 5.0f });
	if (!m_gemsTexture.loadFromFile("ASSETS\\IMAGES\\gems.png"))
	{
		std::cout << "problem with gems" << std::endl;
	}
	for (int i = 0; i < GEM_SLOTS; i++)
	{
		m_gemsSprites[i].setTexture(m_gemsTexture,true);
		m_gemsSprites[i].setTextureRect(sf::IntRect{ sf::Vector2i{i * 32,0},sf::Vector2i{32,32} });
	}
	
	setupContracts();
}


Market::~Market()
{
}

void Market::render(sf::RenderWindow & t_window)
{
	t_window.draw(m_backgroundSprite);
	t_window.draw(m_menuIconSprite);
	t_window.draw(m_titleText);



	m_holdingText.setString("Space Credits \n " + std::to_string(Game::s_credits));
	m_holdingText.setPosition(sf::Vector2f{ 100.0f, 110.0f });
	t_window.draw(m_holdingText);
	int foundInHand = 0;
	for (int i = 0; i < GEM_SLOTS; i++)
	{
		if (Game::s_gems[i] > 0)
		{
			m_gemsSprites[i].setPosition(sf::Vector2f{ 100.0f, CONTRACT_OFFSET_Y + foundInHand * MARKET_LINE_GAP });
			t_window.draw(m_gemsSprites[i]);
			m_holdingText.setString(std::to_string(Game::s_gems[i]));
			m_holdingText.setPosition(sf::Vector2f{ 140.0f, CONTRACT_OFFSET_Y + foundInHand++ * MARKET_LINE_GAP });
			t_window.draw(m_holdingText);
		}
	}
	
	int found = 0;  // only display top 5
	int contractNo = 0; // don't check past all contracts
	while (found < 5 && contractNo < MAX_CONTRACTS)
	{
		if (!m_contracts[contractNo].m_completed)
		{
			int requiredGemNo = 0;
			for (int i = 0; i < GEM_SLOTS; i++)
			{
				for (int j = 0; j < m_contracts[contractNo].m_requiredGems[i]; j++)
				{
					m_gemsSprites[i].setPosition(sf::Vector2f{ CONTRACT_OFFSET_X + INTER_GEM_GAP * requiredGemNo++, CONTRACT_OFFSET_Y + found * MARKET_LINE_GAP });
					t_window.draw(m_gemsSprites[i]);
				}
			}
			if (m_contracts[contractNo].check(Game::s_gems))
			{
				m_valueText.setFillColor(sf::Color::White);
				if (m_marketSelection == found)
				{
					m_valueText.setFillColor(sf::Color::Yellow);
				}
			}
			else
			{
				m_valueText.setFillColor(sf::Color{ 128,128,128,255 });
			}
			m_valueText.setString(std::to_string(m_contracts[contractNo].m_value));
			m_valueText.setPosition(sf::Vector2f{ VALUE_OFFSET_X , CONTRACT_OFFSET_Y + found * MARKET_LINE_GAP });
			t_window.draw(m_valueText);
			found++;
		}
		contractNo++;;
	}

}

void Market::update(sf::Time t_deltaTime, sf::RenderWindow & t_window)
{
}

void Market::processEvents(const std::optional<sf::Event> t_event)
{

	if (t_event->is<sf::Event::MouseButtonReleased>())
	{
		const sf::Event::MouseButtonReleased* newMouseRelease = t_event->getIf < sf::Event::MouseButtonReleased>();

		if (newMouseRelease->position.x > 750 && newMouseRelease->position.y < 50)
		{
			Game::s_currentGameState = GameState::Hub;
		}
		if (m_marketSelection != -1)
		{
			int contractNo = 0;
			int currentContract = -1;
			while (contractNo < MAX_CONTRACTS)
			{
				if (!m_contracts[contractNo].m_completed)
				{
					currentContract++;
					if (m_contracts[contractNo].check(Game::s_gems))
					{
						if (currentContract == m_marketSelection)
						{
							m_contracts[contractNo].complete();
							m_completeSound.play();
						}
					}
				}
				contractNo++;
			}
		}
	}

	if (t_event->is<sf::Event::MouseMoved>())
	{
		const sf::Event::MouseMoved* newMouseMove = t_event->getIf<sf::Event::MouseMoved>();

		m_marketSelection = -1;
		if (newMouseMove->position.x > CONTRACT_OFFSET_X && newMouseMove->position.x < VALUE_OFFSET_X + 50.0f)
		{
			if (newMouseMove->position.y > CONTRACT_OFFSET_Y && newMouseMove->position.x < CONTRACT_OFFSET_Y + 5 * MARKET_LINE_GAP)
			{
				m_marketSelection = static_cast<int>((newMouseMove->position.y - CONTRACT_OFFSET_Y) / MARKET_LINE_GAP);
			}
		}

	}
}

void Market::initialise(sf::Font & t_font)
{
	m_font = t_font;
	setupText(m_titleText, "Market", { 350,50 });
	setupText(m_valueText, "", { 0,0 });
	setupText(m_holdingText, "", { 0,0 });
	if (!m_completeSoundBuffer.loadFromFile("assets\\audio\\printer.wav"))
	{
		std::cout << "problem loading laser sound" << std::endl;
	}
	m_completeSound.setBuffer(m_completeSoundBuffer);
	
}

void Market::setupText(sf::Text & t_text, std::string t_string, sf::Vector2f t_position)
{
	t_text.setFont(m_font);
	t_text.setCharacterSize(24);
	t_text.setString(t_string);
	t_text.setFillColor(sf::Color::White);
	t_text.setPosition(t_position);

}

void Market::setupContracts()
{
	int variation;
	for (int i = 0; i < 10; i++)
	{
		variation = rand() % 2;
		m_contracts[i * 10 + 0].initialise(1, 2, 0, variation, 0, 1000);
		m_contracts[i * 10 + 1].initialise(1, 3, 0, 2, 0, 2000);
		m_contracts[i * 10 + 2].initialise(0, variation, 2, 2, 0, 3000);
		m_contracts[i * 10 + 3].initialise(1, 3, 0, 2, 0, 4000);
		m_contracts[i * 10 + 4].initialise(1, 3, 0, 2, 0, 4000);
		variation = rand() % 4;
		m_contracts[i * 10 + 5].initialise(0, 0, 3, variation, 1, 7000);
		m_contracts[i * 10 + 6].initialise(1, 3, 0, 0, 4, 8000);
		m_contracts[i * 10 + 7].initialise(1, 0, 1, 0, 2, 4000);
		m_contracts[i * 10 + 8].initialise(1, variation, 1, 0, 3, 6000);
		m_contracts[i * 10 + 9].initialise(1, 1, 0, 0, variation, 2000);
	}
}
