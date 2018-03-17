#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "GUI/Widget.hpp"

#include <string>
#include <functional>

class Button : public Widget
{
public:
	Button(std::string name, sf::Vector2f size);
	Button(std::string name, sf::Vector2f size, std::function<void()> func);
	
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	
	sf::Vector2f getPosition() override;
	sf::Vector2f getSize() override;
	void setPosition(sf::Vector2f position) override;
	void setSize(sf::Vector2f size);
	
	void setFunc(std::function<void()> func);

private:
	sf::RectangleShape buttonRect;
	std::function<void()> funcCallback;
};

#endif //BUTTON_HPP