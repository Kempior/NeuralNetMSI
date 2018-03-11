#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "SFML/Graphics.hpp"

#include <string>
#include <functional>

class Button
{
public:
	Button();
	Button(sf::Vector2f pos, sf::Vector2f size, std::string text);
	Button(sf::Vector2f pos, sf::Vector2f size, std::string text, std::function<void()> func);
	
	void setPosition(sf::Vector2f pos);
	void setSize(sf::Vector2f size);
	void setColor(sf::Color color);
	void setText(std::string text);
	
	void setFunc(std::function<void()> func);
	
	void handleEvent(sf::Event& event);
	void draw(sf::RenderWindow& window);
	
private:
	void setTextPosition();
	void setTextOrigin();

private:
	std::function<void()> funcCallback;
	sf::RectangleShape buttonRect;
	sf::Text buttonText;
};

#endif //BUTTON_HPP