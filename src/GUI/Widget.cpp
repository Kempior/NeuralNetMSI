#include "GUI/Widget.hpp"

bool Widget::isModified = false;

Widget::Widget(std::string widgetName):
name(widgetName)
{
	
}

void Widget::handleEvent(const sf::Event& event)
{
	for(auto widget : childWidgets)
	{
		widget->handleEvent(event);
	}
}

void Widget::update(float dt)
{
	for(auto widget : childWidgets)
	{
		widget->update(dt);
	}
}

void Widget::draw(sf::RenderWindow& window)
{
	for(auto widget : childWidgets)
	{
		widget->draw(window);
	}
}

Widget::~Widget()
{
	for(unsigned int i = 0; i < childWidgets.size(); ++i)
	{
		delete childWidgets[i];
	}
}

void Widget::addChildWidget(Widget* widget)
{
	widget->parent = this;
	childWidgets.push_back(widget);
	widget->recalculatePosition();
}

void Widget::recalculatePosition()
{
	if(parent == nullptr)
	{
		return;
	}
	
	sf::Vector2f newPosition;
	
	sf::Vector2f parentPos = parent->getPosition();
	sf::Vector2f parentSize = parent->getSize();
	
	newPosition = parentPos + sf::Vector2f(parentSize.x * anchor.x + anchorOffset.x, parentSize.y * anchor.y + anchorOffset.y);
	
	sf::Vector2f size = getSize();
	
	sf::Vector2f offset;
	offset.x += ((anchorMode & 8) != 0 ? size.x : 0);
	offset.x += ((anchorMode & 4) != 0 ? size.x / 2.0f : 0);
	
	offset.y += ((anchorMode & 2) != 0 ? size.y : 0);
	offset.y += ((anchorMode & 1) != 0 ? size.y / 2.0f : 0);
	
	newPosition -= offset;
	
	setPosition(newPosition);
	
	for(auto widget : childWidgets)
	{
		widget->recalculatePosition();
	}
}

void Widget::setAnchor(sf::Vector2f anchor, sf::Vector2f anchorOffset, AnchorPoint anchorMode)
{
	this->anchor = anchor;
	this->anchorOffset = anchorOffset;
	this->anchorMode = anchorMode;
	
	recalculatePosition();
}

void Widget::setIsActive(bool isActive)
{
	newIsActive = isActive;
	Widget::isModified = true;
}

void Widget::updateStatus()
{
	isActive = newIsActive;
	for(auto widget : childWidgets)
	{
		widget->updateStatus();
	}
}