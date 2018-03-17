#include "Application.hpp"

#include "NeuralNet.hpp"
#include "FileManip.hpp"

#include "ServiceLocator/Locator.hpp"
#include "ServiceLocator/DefaultResourceFont.hpp"

#include "GUI/Widgets.hpp"

#include <iostream>
#include <sstream>

Application::Application()
{
	window.create(sf::VideoMode(1280, 720), "Neural Net");
	canvas = new Canvas(window);
	
	Locator::provideFont(new DefaultResourceFont());

	Button* b = new Button("Button", sf::Vector2f(100.0f, 40.0f));
	b->setAnchor(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(10.0f, 10.0f), UPPER_LEFT);
	canvas->addChildWidget(b);

	Label* l = new Label("Label", "Hello");
	l->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	b->addChildWidget(l);
	
	TextField* tf = new TextField("TextField", sf::Vector2f(200.0f, 30.0f));
	tf->setAnchor(sf::Vector2f(0.05f, 0.5f), sf::Vector2f(), LEFT);
	canvas->addChildWidget(tf);
	
	Label* data = new Label("DataLabel", "");
	data->setAnchor(sf::Vector2f(1.0f, 0.0f), sf::Vector2f(-10.0f, 10.0f), UPPER_RIGHT);
	canvas->addChildWidget(data);
	
	b->setFunc([this, b, data]() {
		b->getWidget<Label>("Label")->setText("Hello Hello");
		
		this->neuralNet->LoadWeights(LoadFile("weights.txt", 3, 3));
		
		std::ostringstream os;
		
		std::vector<std::vector<float>> weights = this->neuralNet->Weights();
		os << weights[0][0] << ' ' << weights[0][1] << ' ' << weights[0][2] << "\n";
		os << weights[1][0] << ' ' << weights[1][1] << ' ' << weights[1][2] << "\n";
		os << weights[2][0] << ' ' << weights[2][1] << ' ' << weights[2][2] << "\n\n";
		
		examples = LoadFile("trainingValues.txt", 3, 2);
		os << examples[0][0] << ' ' << examples[0][1] << "\n";
		os << examples[1][0] << ' ' << examples[1][1] << "\n";
		os << examples[2][0] << ' ' << examples[2][1] << "\n";
		
		data->setText(os.str());
		neuralNet->Learn(examples);
	});
	
	Label* uL = new Label("UpdateLabel", "");
	uL->setAnchor(sf::Vector2f(0.05f, 0.75f), sf::Vector2f(), LEFT);
	canvas->addChildWidget(uL);
	
	uL->setTextSourceLambda([this]() -> std::string {
		std::ostringstream os;
		
		os << this->time;
		
		return os.str();
	});
	
	neuralNet = new NeuralNet(2, 3);
}

Application::~Application()
{
	delete canvas;
	delete neuralNet;
	
	Locator::terminate();
}

void Application::run()
{
	sf::Clock clock;
	
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			canvas->handleEvent(event);
			
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
			
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
			
			if(event.type == sf::Event::Resized)
			{
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			}
		}
		
		time = clock.getElapsedTime().asSeconds();
		canvas->update(0.0f);
		
		window.clear();
		
		canvas->draw(window);
		
		window.display();
	}
}