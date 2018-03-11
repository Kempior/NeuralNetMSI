#include "Button.hpp"

#include "ServiceLocator/Locator.hpp"

Button::Button()
{
	buttonText.setFont(Locator::getResourceFont()->getFont());
	buttonText.setCharacterSize(16);
	setTextOrigin();
}

Button::Button(sf::Vector2f pos, sf::Vector2f size, std::string text): Button()
{
	setPosition(pos);
	setSize(size);
	setText(text);
}

Button::Button(sf::Vector2f pos, sf::Vector2f size, std::string text, std::function<void()> func) : Button()
{
	setPosition(pos);
	setSize(size);
	setText(text);
	funcCallback = func;
}

void Button::setPosition(sf::Vector2f pos)
{
	buttonRect.setPosition(pos);
	setTextPosition();
}

void Button::setSize(sf::Vector2f size)
{
	buttonRect.setSize(size);
}

void Button::setColor(sf::Color color)
{
	buttonRect.setFillColor(color);
}

void Button::setText(std::string text)
{
	buttonText.setString(text);
	setTextOrigin();
}

void Button::setFunc(std::function<void()> func)
{
	funcCallback = func;
}

void Button::handleEvent(sf::Event& event)
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
}

void Button::draw(sf::RenderWindow& window)
{
	window.draw(buttonRect);
	window.draw(buttonText);
}

void Button::setTextPosition()
{
	buttonText.setPosition(buttonRect.getPosition().x + buttonRect.getSize().x / 2.0f, buttonRect.getPosition().y + buttonRect.getSize().y / 2.0f);
}

void Button::setTextOrigin()
{
	sf::FloatRect bounds = buttonText.getLocalBounds();
	buttonText.setOrigin(bounds.top + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	setTextPosition();
}