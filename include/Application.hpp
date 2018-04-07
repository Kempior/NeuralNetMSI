#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "SFML/Graphics.hpp"
#include "NeuralNet.hpp"

#include <vector>
#include <random>

class Canvas;

class Application
{
public:
	Application();
	~Application();
	
	void run();
	
private:
	void setupGUI();
	void setupBoxes();
	void setupDataBox();
	void setupInfoBox();
	void setupHistoryGraph();
	void setupGeometryGraph();
	
private:
	sf::RenderWindow window;
	Canvas* canvas;
	
	NeuralNet *neuralNet;
	std::vector<std::vector<float>> weights, examples;
	std::vector<float> predictPoint;
	bool slowMotion = false, isLearning = false;
	float speedMult = 1.0f;

	std::minstd_rand0 generator;
	std::uniform_int_distribution<int> dist;
};

#endif //APPLICATION_HPP