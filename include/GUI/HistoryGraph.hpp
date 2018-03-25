#ifndef HISTORYGRAPH_HPP
#define HISTORYGRAPH_HPP

#include "GUI/Widget.hpp"

#include <vector>

class HistoryGraph : public Widget
{
public:
	HistoryGraph(std::string name, sf::Vector2f size);
	
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	
	sf::Vector2f getPosition() override;
	sf::Vector2f getSize() override;
	void setPosition(sf::Vector2f position) override;
	
	void addWeights(const std::vector<std::vector<float>>& weight);
	void setWeights(const std::vector<std::vector<std::vector<float>>>& weight);
	
	void reset();
	
private:
	void setView();
	
private:
	sf::Vector2f position;
	sf::Vector2f size;
	
	sf::View graphView;
	
	sf::VertexArray weights[9];
	static sf::Color colors[9];
	int iteration = 0;
	float maxValue = 0.0f;
	sf::RectangleShape horizontalLine, verticalLine;
};

#endif //HISTORYGRAPH_HPP