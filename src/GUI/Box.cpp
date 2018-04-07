#include "GUI/Box.hpp"

Box::Box(std::string name, sf::FloatRect rect): Widget(name),
rect(rect)
{
	
}

void Box::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::Resized)
	{
		recalculateSize();
	}
	
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

void Box::recalculateSize()
{
	if(!parent)
	{
		return;
	}
	
	sf::Vector2f parentSize = parent->getSize();
	
	sf::Vector2f tl = sf::Vector2f(rect.left * parentSize.x, rect.top * parentSize.y);
	sf::Vector2f br = sf::Vector2f(rect.width * parentSize.x, rect.height * parentSize.y);
	
	size = br - tl;
	
	recalculatePosition();
}