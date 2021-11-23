#pragma once

#include <deque>
#include <string>
#include "SFML/Graphics.hpp"
#include <functional>

namespace sf
{
	class Packet;
}

class ChatBox
{
private:
	const std::size_t historyLength;

	const int charSize;
	const float length;




	sf::Text historyText;
	std::deque<sf::String> history;

	std::string buffer;

	sf::String s;
	std::string b;


	std::function<void(const std::string& s)> onEnter;
public:
	ChatBox(const sf::Vector2f pos, const float length, const float thickness, const int charSize, const std::size_t historyLength, const sf::Font& font);
	~ChatBox();

	sf::RectangleShape box1;  //histories go here
	sf::RectangleShape box2;  //buffer goes here

	sf::RectangleShape box3;
	const float thickness;
	sf::Text text;
	sf::Text buttonText;

	sf::Sprite image;

	bool isVisible = true;

	void setOutlineColor(const sf::Color& color);
	void setFillColor(const sf::Color& color);
	void setCharColor(const sf::Color& color);

    void setBox1Size(int x, int y);
    void setBox23Size(int x, int y);

    void setBox1Position(int x, int y);
    void setBox2Position(int x, int y);
    void setBox3Position(int x, int y);

    void setTextSize(int x, int y);
    void setTextPosition(int x, int y);
    void setButtonTextPosition(int x, int y);

    void setVisible();

	void update();
	void handleEvent(sf::Event& event, sf::RenderWindow& window, int id);

	void push(std::wstring& s);
	void connectOnEnter(std::function<void(const std::string& s)> func);

	void draw(sf::RenderWindow& window);

};
