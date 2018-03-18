#include "GUI/TextField.hpp"

#include "GUI/Label.hpp"

TextField::TextField(std::string name, sf::Vector2f size): Widget(name)
{
	setSize(size);
	textFieldRect.setFillColor(sf::Color::White);
	textFieldRect.setOutlineColor(sf::Color::Red);
	
	text = new Label(name + "Label", "Enter text...");
	text->setColor(sf::Color::Black);
	text->setAnchor(sf::Vector2f(0.05f, 0.5f), sf::Vector2f(), LEFT);
	addChildWidget(text);
}

void TextField::handleEvent(const sf::Event& event)
{
	if(isActive)
	{
		if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			if(	event.mouseButton.x < textFieldRect.getPosition().x ||
				event.mouseButton.x > textFieldRect.getPosition().x + textFieldRect.getSize().x ||
				event.mouseButton.y < textFieldRect.getPosition().y ||
				event.mouseButton.y > textFieldRect.getPosition().y + textFieldRect.getSize().y)
			{
				if(isSelected)
				{
					isSelected = false;
					textFieldRect.setOutlineThickness(0.0f);
					if(validateFunc && !validateFunc(inputedText))
					{
						inputedText = oldText;
						text->setText(inputedText);
					}
					else
					{
						if(dataUpdateFunc)
						{
							dataUpdateFunc();
						}
					}
				}
			}
			else
			{
				isSelected = true;
				textFieldRect.setOutlineThickness(2.0f);
				oldText = inputedText;
				inputedText = "";
			}
		}
		
		if(isSelected && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
		{
			isSelected = false;
			textFieldRect.setOutlineThickness(0.0f);
			inputedText = oldText;
			text->setText(inputedText);
		}
		
		if(isSelected && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
		{
			isSelected = false;
			textFieldRect.setOutlineThickness(0.0f);
			if(validateFunc && !validateFunc(inputedText))
			{
				inputedText = oldText;
				text->setText(inputedText);
			}
			else
			{
				if(dataUpdateFunc)
				{
					dataUpdateFunc();
				}
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
	if(isActive && textSourceFunc && !isSelected)
	{
		inputedText = textSourceFunc();
		text->setText(inputedText);
	}
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

void TextField::setValidateFunc(std::function<bool(std::string)> validateFunc)
{
	this->validateFunc = validateFunc;
}

void TextField::setTextSourceFunc(std::function<std::string()> textSourceFunc)
{
	this->textSourceFunc = textSourceFunc;
}

void TextField::setDataUpdateFunc(std::function<void()> dataUpdateFunc)
{
	this->dataUpdateFunc = dataUpdateFunc;
}

void TextField::setText(std::string str)
{
	if(validateFunc && !validateFunc(str))
	{
		return;
	}
	
	inputedText = str;
	text->setText(inputedText);
}

std::string TextField::getText()
{
	return inputedText;
}