#include "GUI/Box.hpp"

Box::Box(std::string name, sf::Vector2f size): Widget(name),
size(size)
{
	
}

void Box::handleEvent(const sf::Event& event)
{
	if(isActive)
	{
		Widget::handleEvent(event);
	}
}

void Box::update(float dt)
{
	if(isActive)
	{
		Widget::update(dt);
	}
}

void Box::draw(sf::RenderWindow& window)
{
	if(isActive)
	{
		Widget::draw(window);
	}
}

sf::Vector2f Box::getPosition()
{
	return position;
}

sf::Vector2f Box::getSize()
{
	return size;
}

void Box::setPosition(sf::Vector2f position)
{
	this->position = position;
}