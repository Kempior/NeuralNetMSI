#include "Application.hpp"

#include "Button.hpp"
#include "NeuralNet.hpp"
#include "FileManip.hpp"

#include "ServiceLocator/Locator.hpp"
#include "ServiceLocator/DefaultResourceFont.hpp"

#include <iostream>

Application::Application()
{
	window.create(sf::VideoMode(1280, 720), "Neural Net", sf::Style::Titlebar | sf::Style::Close);
	
	Locator::provideFont(new DefaultResourceFont());
	
	button = new Button();
	
	button->setPosition(sf::Vector2f(10.0f, 10.0f));
	button->setSize(sf::Vector2f(100.0f, 40.0f));
	button->setText("Button");
	button->setColor(sf::Color::Blue);
	
	neuralNet = new NeuralNet(2, 3);
	
	button->setFunc([this]() {
			this->button->setText("Hello");
			
			this->neuralNet->LoadWeights(LoadFile("../../weights.txt", 3, 3));
			examples = LoadFile("../../trainingValues.txt", 3, 2);
			
			neuralNet->Learn(examples);
		});
}

Application::~Application()
{
	delete button;
	delete neuralNet;
	
	Locator::terminate();
}

void Application::run()
{
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			button->handleEvent(event);
			
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
			
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
		}
		
		window.clear();
		
		button->draw(window);
		
		window.display();
	}
}