#ifndef GEOMETRYGRAPH_HPP
#define GEOMETRYGRAPH_HPP

#include "GUI/Widget.hpp"

class GeometryGraph : public Widget
{
public:
	GeometryGraph(std::string name, sf::Vector2f size);
	
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	
	sf::Vector2f getPosition() override;
	sf::Vector2f getSize() override;
	void setPosition(sf::Vector2f position) override;
	
	void setPoints(const std::vector<std::vector<float>>& points);
	void setPredictPoint(const std::vector<float>& point);
	void setWeights(const std::vector<std::vector<float>>& weights);
	
private:
	void setView();
	
private:
	sf::Vector2f position;
	sf::Vector2f size;
	
	sf::View graphView;
	
	float maxValue = 0.0f;
	sf::RectangleShape horizontalLine, verticalLine, lines[3];
	sf::CircleShape points[3];
	sf::CircleShape predictPoint;
};

#endif //GEOMETRYGRAPH_HPP