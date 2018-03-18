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
	void setupGUI();
	
private:
	sf::RenderWindow window;
	Canvas* canvas;
	
	NeuralNet *neuralNet;
	std::vector<std::vector<float>> weights, examples;

	std::minstd_rand0 generator;
	std::uniform_int_distribution<int> dist;
};

#endif //APPLICATION_HPP