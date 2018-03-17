#include "GUI/TextField.hpp"

#include "GUI/Label.hpp"

TextField::TextField(std::string name, sf::Vector2f size): Widget(name)
{
	setSize(size);
	textFieldRect.setFillColor(sf::Color::White);
	
	text = new Label(name + "Label", "Enter text...");
	text->setColor(sf::Color::Black);
	text->setAnchor(sf::Vector2f(0.05f, 0.5f), sf::Vector2f(), LEFT);
	addChildWidget(text);
}

void TextField::handleEvent(const sf::Event& event)
{
	if(isActive)
	{
		if(event.type == sf::Event::MouseButtonPressed  && event.mouseButton.button == sf::Mouse::Left)
		{
			if(	event.mouseButton.x < textFieldRect.getPosition().x ||
				event.mouseButton.x > textFieldRect.getPosition().x + textFieldRect.getSize().x ||
				event.mouseButton.y < textFieldRect.getPosition().y ||
				event.mouseButton.y > textFieldRect.getPosition().y + textFieldRect.getSize().y)
			{
				isSelected = false;
			}
			else
			{
				isSelected = true;
			}
		}

		if(isSelected && event.type == sf::Event::TextEntered)
		{
			if(event.text.unicode == 8)
			{
				if(!inputedText.empty())
				{
					inputedText.pop_back();
				}
			}
			else
			{
				inputedText += static_cast<char>(event.text.unicode);
			}
			
			text->setText(inputedText);
		}
		
		Widget::handleEvent(event);
	}
}

void TextField::update(float dt)
{
	return;
}

void TextField::draw(sf::RenderWindow& window)
{
	if(isActive)
	{
		window.draw(textFieldRect);
		
		Widget::draw(window);
	}
}

sf::Vector2f TextField::getPosition()
{
	return textFieldRect.getPosition();
}

sf::Vector2f TextField::getSize()
{
	return textFieldRect.getSize();
}

void TextField::setPosition(sf::Vector2f position)
{
	textFieldRect.setPosition(position);
}

void TextField::setSize(sf::Vector2f size)
{
	textFieldRect.setSize(size);
	recalculatePosition();
}
	
std::string TextField::getText()
{
	return inputedText;
}