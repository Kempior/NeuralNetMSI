#include "Application.hpp"

#include "Button.hpp"

#include "ServiceLocator/Locator.hpp"
#include "ServiceLocator/DefaultResourceFont.hpp"

Application::Application()
{
	window.create(sf::VideoMode(1280, 720), "Neural Net", sf::Style::Titlebar | sf::Style::Close);
	
	Locator::provideFont(new DefaultResourceFont());
	
	button = new Button();
	
	button->setPosition(sf::Vector2f(10.0f, 10.0f));
	button->setSize(sf::Vector2f(100.0f, 40.0f));
	button->setText("Button");
	button->setColor(sf::Color::Blue);
	
	button->setFunc([this]() {
			this->button->setText("Hello");
		});
}

Application::~Application()
{
	delete button;
	
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