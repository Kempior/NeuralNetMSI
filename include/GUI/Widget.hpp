#ifndef WIDGET_HPP
#define WIDGET_HPP

#include "SFML/Graphics.hpp"

#include <vector>

enum AnchorPoint : short
{
	UPPER_LEFT = 0,
	LEFT = 1,
	LOWER_LEFT = 2,
	UP = 4,
	MIDDLE = 5,
	DOWN = 6,
	UPPER_RIGHT = 8,
	RIGHT = 9,
	LOWER_RIGHT = 10
};

class Widget
{
public:
	Widget(std::string widgetName);
	virtual ~Widget();
	
	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	
	void addChildWidget(Widget* widget);
	
	template<typename T>
	T* getWidget(std::string name);
	
	virtual sf::Vector2f getPosition() = 0;
	virtual sf::Vector2f getSize() = 0;
	virtual void setPosition(sf::Vector2f position) = 0;
	void recalculatePosition();
	void setAnchor(sf::Vector2f anchor, sf::Vector2f anchorOffset, AnchorPoint anchorMode);
	
public:
	bool isActive = true;
	
protected:
	Widget* parent = nullptr;
	std::vector<Widget*> childWidgets;
	
private:
	std::string name;

	sf::Vector2f anchor;
	sf::Vector2f anchorOffset;
	AnchorPoint anchorMode = UPPER_LEFT;
};

template<typename T>
inline T* Widget::getWidget(std::string name)
{
	if(this->name == name)
	{
		return reinterpret_cast<T*>(this);
	}
	
	for(auto widget : childWidgets)
	{
		Widget* w = widget->getWidget<Widget>(name);
		if(w)
		{
			return reinterpret_cast<T*>(w);
		}
	}
	
	return nullptr;
}

#endif //WIDGET_HPP