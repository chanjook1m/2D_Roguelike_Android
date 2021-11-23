#include "InputBox.h"
#include <iostream>
#include <android/log.h>

InputBox::InputBox(const sf::Vector2f pos, const float width_, const float height_, const int charSize_,
	const std::size_t historyLength_, const sf::Font& font, int type_) :
	charSize(charSize_), historyLength(historyLength_), type(type_)
{
	box.setPosition(pos);
	box.setSize(sf::Vector2f(width_, height_));


	text.setFont(font);
	text.setCharacterSize(charSize);
	text.setFillColor(sf::Color::Black);
	text.setScale(1, 1);
	text.setPosition(pos.x + 30, pos.y);

	p.setFont(font);
    p.setCharacterSize(charSize);
    p.setFillColor(sf::Color::White);
    p.setPosition(pos.x + 30, pos.y);
    p.setString("AAA");

	historyText.setFont(font);
	historyText.setCharacterSize(charSize);

	sf::Texture texture;
        texture.loadFromFile("image.png");
        image.setTexture(texture);
        image.setScale(0.5,0.5);
        image.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}

InputBox::~InputBox(void)
{
}

void InputBox::setBoxSize(int x, int y)
{
    //box.setSize(sf::Vector2f(120 * x, 50 * y));
    box.setScale(x, y);
}

void InputBox::setBoxPosition(int x, int y)
{
    box.setPosition(sf::Vector2f(x, y));
}

void InputBox::setTextSize(int x, int y)
{
    text.setScale(x, y);
}

void InputBox::setTextPosition(int x, int y)
{
    text.setPosition(sf::Vector2f(x,y));
}

sf::Vector2f InputBox::getBoxPosition()
{
    return box.getPosition();
}

sf::Vector2f InputBox::getTextPosition()
{
    return text.getPosition();
}

void InputBox::update()
{
	//text.setString(buffer);
	if (type == 0)
		text.setString(s);
	else if (type == 1)
	{
		sf::String new_s;
		for (int i = 0; i < s.getSize(); i++)
		{
			new_s += "*";
		}
		text.setString(new_s);
	}
}

void InputBox::handleEvent(sf::Event& event, sf::RenderWindow& window)
{
	if (event.type == sf::Event::TextEntered && isSelected)
	{
		char code = static_cast<char>(event.text.unicode);

		if (event.text.unicode == 13)//enter
		{
			onEnter(buffer);
			buffer.clear();
		}

		else if (code != '\b' && code != 10 && s.getSize() < 6)
		{
            __android_log_print(ANDROID_LOG_INFO, "OBJECT_UPDATE", "window code: %d", code);
			s += event.text.unicode;//buffer.push_back(code);
		}
		else if (code == 10)
		{
			/*if (buffer.size() > 0)
				buffer.pop_back();*/
			if (s.getSize() > 0) {
				//std::cout << "���ڿ����� : " << s.getSize() << std::endl;
				s.erase(s.getSize() - 1); //���������ڸ� �����.
			}
		}
		else
		{
		    __android_log_print(ANDROID_LOG_INFO, "OBJECT_UPDATE", "window code: %d", code);
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		isSelected = false;
		if (event.key.code == sf::Mouse::Left)
		{
			if (sf::Mouse::getPosition(window).x >= box.getPosition().x
				&& sf::Mouse::getPosition(window).x <= box.getPosition().x + box.getSize().x
				&& sf::Mouse::getPosition(window).y >= box.getPosition().y
				&& sf::Mouse::getPosition(window).y <= box.getPosition().y + box.getSize().y)
				// The box has been selected
				// Toggle the boolean
			{
				isSelected = !isSelected;
			}
		}
	}
	else if (event.type == sf::Event::TouchBegan)
    	{
    	    if (event.touch.finger == 0)
            {
                int posX;
                int posY;
                int dx = 0;
                int dy = 0;
                int width = window.getSize().x;
                int height = window.getSize().y;
                __android_log_print(ANDROID_LOG_INFO, "OBJECT_UPDATE", "window size: %d : %d", width, height);
                if (width == 768 && height == 1280)
                {

                    posX = event.touch.x + 100;
                    posY = event.touch.y - 260;
                }
                else if (width == 1080 && height == 1776)
                {
                    dy = 60;

                    posX = event.touch.x - 50;
                    posY = event.touch.y - 500;

                }

                p.setPosition(posX, posY);

                isSelected = false;
                if (posX >= box.getPosition().x
                    && posX <= box.getPosition().x + box.getSize().x
                    && posY >= box.getPosition().y
                    && posY <= box.getPosition().y + box.getSize().y + dy)
                    // The box has been selected
                    // Toggle the boolean
                    {
                        isSelected = !isSelected;
                    }
            }
    	}
	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSpace))
	//{
	//	if (s.getSize() > 0) {
	//		//std::cout << "���ڿ����� : " << s.getSize() << std::endl;
	//		s.erase(s.getSize() - 1); //���������ڸ� �����.
	//	}
	//}
}

void InputBox::push(const std::string& s)
{
	if (s.size() > 0)
	{
		history.push_front(s);
		if (history.size() > historyLength)
			history.pop_back();
	}
}

void InputBox::connectOnEnter(std::function<void(const std::string& s)> func)
{
	onEnter = func;
}

void InputBox::draw(sf::RenderWindow& window)
{
	window.draw(box);
	window.draw(text);
	//window.draw(image);
	//window.draw(p);

	float x = box.getGlobalBounds().left;
	float y = box.getGlobalBounds().top + box.getGlobalBounds().height - charSize;

	for (std::size_t i = 0; i < history.size(); ++i)
	{
		historyText.setString(history[i]);
		historyText.setPosition(x, y - i * charSize);
		window.draw(historyText);
	}
}


void InputBox::setOutlineColor(const sf::Color& color)
{
	box.setOutlineColor(color);
}

void InputBox::setFillColor(const sf::Color& color)
{
	box.setFillColor(color);
}

void InputBox::setCharColor(const sf::Color& color)
{
	text.setFillColor(color);
	historyText.setFillColor(color);
}
