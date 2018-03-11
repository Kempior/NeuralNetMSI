#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "SFML/Graphics.hpp"

class Button;

class Application
{
public:
	Application();
	~Application();
	
	void run();
	
private:
	sf::RenderWindow window;
	Button* button;
};

#endif //APPLICATION_HPP