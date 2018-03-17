#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "SFML/Graphics.hpp"
#include "NeuralNet.hpp"

#include <vector>

class Canvas;

class Application
{
public:
	Application();
	~Application();
	
	void run();
	
private:
	sf::RenderWindow window;
	Canvas* canvas;
	
	float time;
	
	NeuralNet *neuralNet;
	std::vector<std::vector<float>> examples;
};

#endif //APPLICATION_HPP