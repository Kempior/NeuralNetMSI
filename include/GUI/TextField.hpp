#ifndef TEXTFIELD_HPP
#define TEXTFIELD_HPP

#include "GUI/Widget.hpp"

#include <string>
#include <functional>

class Label;

class TextField : public Widget
{
public:
	TextField(std::string name, sf::Vector2f size);
	
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	
	sf::Vector2f getPosition() override;
	sf::Vector2f getSize() override;
	void setPosition(sf::Vector2f position) override;
	void setSize(sf::Vector2f size);
	
	std::string getText();

private:
	sf::RectangleShape textFieldRect;
	Label* text;
	bool isSelected = false;
	std::string inputedText;
};

#endif //TEXTFIELD_HPP