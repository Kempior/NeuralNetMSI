#include "GUI/GeometryGraph.hpp"

#include "GUI/Label.hpp"

GeometryGraph::GeometryGraph(std::string name, sf::Vector2f size): Widget(name),
size(size)
{
	horizontalLine.setFillColor(sf::Color::White);
	verticalLine.setFillColor(sf::Color::White);
	
	for(int i = 0; i < 3; ++i)
	{
		lines[i].setFillColor(sf::Color::Cyan);
		points[i].setFillColor(sf::Color::Red);
	}
	predictPoint.setFillColor(sf::Color::Green);
	
	graphView.setViewport(sf::FloatRect(0.3f, 0.0f, 0.7f, 0.6f));
	
	Label* label = new Label("GeometryAxisX", "X");
	label->setAnchor(sf::Vector2f(1.0f, 0.522f), sf::Vector2f(-5.0f, 0.0f), RIGHT);
	label->setColor(sf::Color::White);
	addChildWidget(label);
	
	label = new Label("GeometryAxisY", "Y");
	label->setAnchor(sf::Vector2f(0.485f, 0.0f), sf::Vector2f(0.0f, 0.0f), UP);
	label->setColor(sf::Color::White);
	addChildWidget(label);
}

void GeometryGraph::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::Resized)
	{
		size = parent->getSize();
		recalculatePosition();
	}
	
	Widget::handleEvent(event);
}

void GeometryGraph::update(float dt)
{
	Widget::update(dt);
}

void GeometryGraph::draw(sf::RenderWindow& window)
{
	sf::View current = window.getView();
	
	window.setView(graphView);
	
	window.draw(horizontalLine);
	window.draw(verticalLine);
	for(int i = 0; i < 3; ++i)
	{
		window.draw(lines[i]);
	}
	
	for(int i = 0; i < 3; ++i)
	{
		window.draw(points[i]);
	}
	
	window.draw(predictPoint);
	
	window.setView(current);
	
	Widget::draw(window);
}

sf::Vector2f GeometryGraph::getPosition()
{
	return position;
}

sf::Vector2f GeometryGraph::getSize()
{
	return size;
}

void GeometryGraph::setPosition(sf::Vector2f position)
{
	this->position = position;
}

void GeometryGraph::setPoints(const std::vector<std::vector<float>>& point)
{
	for(int i = 0; i < 3; ++i)
	{
		this->points[i].setPosition(sf::Vector2f(point[i][0], -point[i][1]));
		maxValue = std::max(maxValue, std::max(std::abs(point[i][0]), std::abs(point[i][1])));
	}
	
	setView();
}

void GeometryGraph::setPredictPoint(const std::vector<float>& point)
{
	predictPoint.setPosition(point[0], -point[1]);
	
	setView();
}

void GeometryGraph::setWeights(const std::vector<std::vector<float>>& weights)
{
	for(int i = 0; i < 3; ++i)
	{
		if(weights[i][0] == 0.0f)
		{
			lines[i].setPosition(0.0f, -weights[i][2] / weights[i][1]);
		}
		else
		{
			lines[i].setPosition(weights[i][2] / weights[i][0], 0.0f);
		}
		
		if(weights[i][1] == 0.0f)
		{
			lines[i].setRotation(90.0f);
		}
		else
		{
			lines[i].setRotation(-std::atan(-weights[i][0] / weights[i][1]) * 180.0f / 3.14159f);
		}
	}
	
	setView();
}

void GeometryGraph::setView()
{
	const float lineSize = 0.005f;
	const float radius = 0.01f;
	
	float maxValue = std::max(this->maxValue, std::max(std::abs(predictPoint.getPosition().x), std::abs(predictPoint.getPosition().y)));
	
	horizontalLine.setSize(sf::Vector2f(3.0f * maxValue * size.x / size.y, maxValue * lineSize));
	horizontalLine.setOrigin(1.5f * maxValue * size.x / size.y, maxValue * lineSize / 2.0f);

	verticalLine.setSize(sf::Vector2f(maxValue * lineSize, 3.0f * maxValue));
	verticalLine.setOrigin(maxValue * lineSize / 2.0f, 1.5f * maxValue);
	
	for(int i = 0; i < 3; ++i)
	{
		points[i].setRadius(maxValue * radius);
		points[i].setOrigin(maxValue * radius, maxValue * radius);
	}
	
	predictPoint.setRadius(1.5f * maxValue * radius);
	predictPoint.setOrigin(1.5f * maxValue * radius, 1.5f * maxValue * radius);
	
	for(int i = 0; i < 3; ++i)
	{
		lines[i].setSize(sf::Vector2f(3.0f * std::max(maxValue, std::abs(lines[i].getPosition().x)) * size.x / size.y, maxValue * lineSize));
		lines[i].setOrigin(sf::Vector2f(1.5f * std::max(maxValue, std::abs(lines[i].getPosition().x)) * size.x / size.y, maxValue * lineSize / 2.0f));
	}
	
	graphView.setCenter(0.0f, 0.0f);
	graphView.setSize(2.0f * maxValue * size.x / size.y, 2.0f * maxValue);
	graphView.zoom(1.075f);
}