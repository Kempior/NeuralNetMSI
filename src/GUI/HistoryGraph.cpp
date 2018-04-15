#include "GUI/HistoryGraph.hpp"

#include "GUI/Label.hpp"

sf::Color HistoryGraph::colors[9] = {
	sf::Color::Red,
	sf::Color::Green,
	sf::Color::Blue,
	sf::Color::Yellow,
	sf::Color::Cyan,
	sf::Color::Magenta,
	sf::Color(9, 162, 244, 255),
	sf::Color(252, 120, 5, 255),
	sf::Color(99, 33, 4, 255)
};

HistoryGraph::HistoryGraph(std::string name, sf::Vector2f size): Widget(name),
size(size)
{
	horizontalLine.setFillColor(sf::Color::White);
	verticalLine.setFillColor(sf::Color::White);
	
	graphView.setViewport(sf::FloatRect(0.0f, 0.6f, 1.0f, 0.4f));
	
	for(int i = 0; i < 9; ++i)
	{
		weights[i].setPrimitiveType(sf::LineStrip);
	}
	
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			Label* label = new Label("HGW" + std::to_string(i + 1) + std::to_string(j + 1), "W" + std::to_string(i + 1) + std::to_string(j + 1));
			label->setAnchor(sf::Vector2f(0.0f, 0.1f * (3 * i + j + 1)), sf::Vector2f(0.0f, 0.0f), LEFT);
			label->setColor(colors[3 * i + j]);
			addChildWidget(label);
		}
	}
	
	Label* label = new Label("HistoryAxisX", "Iter");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(-5.0f, 0.0f), RIGHT);
	label->setColor(sf::Color::White);
	addChildWidget(label);
	
	label = new Label("HistoryAxisY", "W");
	label->setAnchor(sf::Vector2f(0.035f, 0.03f), sf::Vector2f(0.0f, 0.0f), DOWN);
	label->setColor(sf::Color::White);
	addChildWidget(label);
}

void HistoryGraph::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::Resized)
	{
		size = parent->getSize();
		recalculatePosition();
	}
	
	Widget::handleEvent(event);
}

void HistoryGraph::update(float dt)
{
	Widget::update(dt);
}

void HistoryGraph::draw(sf::RenderWindow& window)
{
	sf::View current = window.getView();
	
	window.setView(graphView);
	
	window.draw(horizontalLine);
	window.draw(verticalLine);
	for(int i = 0; i < 9; ++i)
	{
		window.draw(weights[i]);
	}
	
	window.setView(current);
	
	Widget::draw(window);
}

sf::Vector2f HistoryGraph::getPosition()
{
	return position;
}

sf::Vector2f HistoryGraph::getSize()
{
	return size;
}

void HistoryGraph::setPosition(sf::Vector2f position)
{
	this->position = position;
}

void HistoryGraph::addWeights(const std::vector<std::vector<float>>& weight)
{
	for(unsigned int i = 0; i < weight.size(); ++i)
	{
		for(unsigned int j = 0; j < weight[i].size(); ++j)
		{
			this->weights[3 * i + j].append(sf::Vertex(sf::Vector2f(iteration, -weight[i][j]), colors[3 * i + j]));
			maxValue = std::max(maxValue, std::abs(weight[i][j]));
		}
	}
	
	setView();
	iteration++;
}

void HistoryGraph::setWeights(const std::vector<std::vector<std::vector<float>>>& weight)
{
	reset();
	
	for(unsigned int i = 0; i < weight.size(); ++i)
	{
		addWeights(weight[i]);
	}
}

void HistoryGraph::reset()
{
	for(int i = 0; i < 9; ++i)
	{
		this->weights[i].clear();
	}
	iteration = 0;
	maxValue = 0.0f;
}

void HistoryGraph::setView()
{
	const float horizontalSize = 0.02f;
	horizontalLine.setSize(sf::Vector2f(iteration, maxValue * horizontalSize));
	horizontalLine.setOrigin(0.0f, maxValue * horizontalSize / 2.0f);
	
	const float verticalSize = 0.001f;
	verticalLine.setSize(sf::Vector2f(iteration * verticalSize, maxValue * 2));
	verticalLine.setOrigin(iteration * verticalSize / 2.0f, maxValue);
	
	graphView.setCenter(iteration / 2.0f, 0.0f);
	graphView.setSize(sf::Vector2f(iteration, 2 * maxValue));
	graphView.zoom(1.075f);
}