#include "GUI/Button.hpp"

Button::Button(std::string name, sf::Vector2f size): Widget(name)
{
	setSize(size);
	buttonRect.setFillColor(sf::Color(34, 145, 32, 255));
}

Button::Button(std::string name, sf::Vector2f size, std::function<void()> func): Button(name, size)
{
	funcCallback = func;
}

void Button::handleEvent(const sf::Event& event)
{
	if(isActive)
	{
		if(event.type == sf::Event::MouseButtonPressed  && event.mouseButton.button == sf::Mouse::Left)
		{
			if(	event.mouseButton.x < buttonRect.getPosition().x ||
				event.mouseButton.x > buttonRect.getPosition().x + buttonRect.getSize().x ||
				event.mouseButton.y < buttonRect.getPosition().y ||
				event.mouseButton.y > buttonRect.getPosition().y + buttonRect.getSize().y)
			{
				return;
			}
			
			funcCallback();
		}
		
		Widget::handleEvent(event);
	}
}

void Button::update(float dt)
{
	if(isActive)
	{
		Widget::update(dt);
	}
	
	return;
}

void Button::draw(sf::RenderWindow& window)
{
	if(isActive)
	{
		window.draw(buttonRect);
		
		Widget::draw(window);
	}
}

sf::Vector2f Button::getPosition()
{
	return buttonRect.getPosition();
}

sf::Vector2f Button::getSize()
{
	return buttonRect.getSize();
}

void Button::setPosition(sf::Vector2f position)
{
	buttonRect.setPosition(position);
}

void Button::setSize(sf::Vector2f size)
{
	buttonRect.setSize(size);
	recalculatePosition();
}

void Button::setFunc(std::function<void()> func)
{
	funcCallback = func;
}