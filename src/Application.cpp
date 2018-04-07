#include "Application.hpp"

#include "NeuralNet.hpp"
#include "FileManip.hpp"

#include "ServiceLocator/Locator.hpp"
#include "ServiceLocator/DefaultResourceFont.hpp"

#include "GUI/Widgets.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <regex>

Application::Application():
generator(std::time(NULL)),
dist(-10, 10)
{
	window.create(sf::VideoMode(1280, 720), "Neural Net");
	canvas = new Canvas(window);
	
	weights.resize(3);
	for(int i = 0; i < 3; ++i)
	{
		weights[i].resize(3);
	}
	
	examples.resize(3);
	for(int i = 0; i < 3; ++i)
	{
		examples[i].resize(2);
	}
	
	predictPoint.resize(2);
	
	generator();
	
	Locator::provideFont(new DefaultResourceFont());
	setupGUI();
	
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
		
		if(isLearning && clock.getElapsedTime().asSeconds() > (1.0f / speedMult))
		{
			clock.restart();
			if(neuralNet->LearnStep())
			{
				isLearning = false;
				canvas->getWidget<Button>("SaveButton")->setIsActive(true);
				canvas->getWidget<Button>("PredictButton")->setIsActive(true);
				canvas->getWidget<TextField>("xTextField")->setIsActive(true);
				canvas->getWidget<TextField>("yTextField")->setIsActive(true);
			}
			else
			{
				canvas->getWidget<HistoryGraph>("HistoryGraph")->addWeights(neuralNet->Weights());
				canvas->getWidget<GeometryGraph>("GeometryGraph")->setWeights(neuralNet->Weights());
			}
		}

		canvas->update(0.0f);
		
		window.clear();
		
		canvas->draw(window);
		
		window.display();
	}
}

void Application::setupGUI()
{
	setupBoxes();
	setupDataBox();
	setupInfoBox();
	setupHistoryGraph();
	setupGeometryGraph();
}

void Application::setupBoxes()
{
	//Data box
	Box* dataBox = new Box("DataBox", sf::FloatRect(0.0f, 0.0f, 0.3f, 0.6f));
	dataBox->setAnchor(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(), UPPER_LEFT);
	canvas->addChildWidget(dataBox);
	dataBox->recalculateSize();
	
	//Info Box
	Box* infoBox = new Box("InfoBox", sf::FloatRect(0.0f, 0.0f, 0.3f, 0.6f));
	infoBox->setAnchor(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(), UPPER_LEFT);
	infoBox->setIsActive(false);
	canvas->addChildWidget(infoBox);
	infoBox->recalculateSize();
	
	//History Box
	Box* historyBox = new Box("HistoryBox", sf::FloatRect(0.0f, 0.6f, 1.0f, 1.0f));
	historyBox->setAnchor(sf::Vector2f(0.0f, 1.0f), sf::Vector2f(), LOWER_LEFT);
	historyBox->setIsActive(false);
	canvas->addChildWidget(historyBox);
	historyBox->recalculateSize();
	
	//Geometry Box
	Box* geometryBox = new Box("GeometryBox", sf::FloatRect(0.3f, 0.0f, 1.0f, 0.6f));
	geometryBox->setAnchor(sf::Vector2f(1.0f, 0.0f), sf::Vector2f(), UPPER_RIGHT);
	geometryBox->setIsActive(false);
	canvas->addChildWidget(geometryBox);
	geometryBox->recalculateSize();
}

void Application::setupDataBox()
{
	Box* dataBox = canvas->getWidget<Box>("DataBox");
	Box* infoBox = canvas->getWidget<Box>("InfoBox");
	
	//Top data buttons
	Button* button = new Button("LoadFromFileButton", sf::Vector2f(120.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(), UPPER_LEFT);
	dataBox->addChildWidget(button);

	Label* label = new Label("LoadFromFileLabel", "Load From File");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this]()
	{
		this->weights = LoadFile("weights.txt", 3, 3);
		this->examples = LoadFile("trainingValues.txt", 3, 2);
	});
	
	////
	button = new Button("RandomButton", sf::Vector2f(120.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(0.0f, 40.0f), UPPER_LEFT);
	dataBox->addChildWidget(button);

	label = new Label("RandomLabel", "Random");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this]()
	{
		for(unsigned int i = 0; i < this->weights.size(); ++i)
		{
			for(unsigned int j = 0; j < this->weights[i].size(); ++j)
			{
				this->weights[i][j] = dist(generator);
			}
		}
		
		for(unsigned int i = 0; i < this->examples.size(); ++i)
		{
			for(unsigned int j = 0; j < this->examples[i].size(); ++j)
			{
				this->examples[i][j] = dist(generator);
			}
		}
	});
	
	////
	button = new Button("NextButton", sf::Vector2f(120.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.97f, 0.02f), sf::Vector2f(), UPPER_RIGHT);
	dataBox->addChildWidget(button);

	label = new Label("NextLabel", "Next");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this, dataBox, infoBox]()
	{
		dataBox->setIsActive(false);
		infoBox->setIsActive(true);
	});
	
	////
	button = new Button("ResetButton", sf::Vector2f(120.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.97f, 0.02f), sf::Vector2f(0.0f, 40.0f), UPPER_RIGHT);
	dataBox->addChildWidget(button);

	label = new Label("ResetLabel", "Reset");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this]()
	{
		for(unsigned int i = 0; i < this->weights.size(); ++i)
		{
			for(unsigned int j = 0; j < this->weights[i].size(); ++j)
			{
				this->weights[i][j] = 0;
			}
		}
		
		for(unsigned int i = 0; i < this->examples.size(); ++i)
		{
			for(unsigned int j = 0; j < this->examples[i].size(); ++j)
			{
				this->examples[i][j] = 0;
			}
		}
	});
	
	////Data weights
	const float offset = 28.0f;
	const float size = 80.0f;
	const float space = 8.0f;
	const int rounding = 6;
	
	auto validateFunc = [](std::string str) -> bool
	{
		return std::regex_match(str, std::regex("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
	};
	
	for(int i = 0; i < 3; ++i)
	{
		label = new Label("W" + std::to_string(i + 1), "W" + std::to_string(i + 1));
		label->setAnchor(sf::Vector2f(0.03f, 0.4f + 0.1f * i), sf::Vector2f(), LEFT);
		dataBox->addChildWidget(label);
		
		TextField* textField;
		
		for(int j = 0; j < 3; ++j)
		{
			textField = new TextField("W" + std::to_string(i + 1) + std::to_string(j + 1), sf::Vector2f(size, 30.0f));
			textField->setAnchor(sf::Vector2f(0.04f, 0.4f + 0.1f * i), sf::Vector2f(offset + j * size + j * space, 0.0f), LEFT);
			dataBox->addChildWidget(textField);
			textField->setTextSourceFunc([this, i, j, rounding]() -> std::string
			{
				return std::to_string(this->weights[i][j]).substr(0, rounding);
			});
			textField->setDataUpdateFunc([this, textField, i, j]()
			{
				this->weights[i][j] = atof(textField->getText().c_str());
			});
			textField->setValidateFunc(validateFunc);
		}
	}
	
	//Data points
	for(int i = 0; i < 3; ++i)
	{
		label = new Label("P" + std::to_string(i + 1), "P" + std::to_string(i + 1));
		label->setAnchor(sf::Vector2f(0.03f, 0.7f + 0.1f * i), sf::Vector2f(), LEFT);
		dataBox->addChildWidget(label);
		
		TextField* textField;
		
		for(int j = 0; j < 2; ++j)
		{
			textField = new TextField("P" + std::to_string(i + 1) + std::to_string(j + 1), sf::Vector2f(size, 30.0f));
			textField->setAnchor(sf::Vector2f(0.04f, 0.7f + 0.1f * i), sf::Vector2f(offset + j * size + j * space, 0.0f), LEFT);
			dataBox->addChildWidget(textField);
			textField->setTextSourceFunc([this, i, j, rounding]() -> std::string
			{
				return std::to_string(this->examples[i][j]).substr(0, rounding);
			});
			textField->setDataUpdateFunc([this, textField, i, j]()
			{
				this->examples[i][j] = atof(textField->getText().c_str());
			});
			textField->setValidateFunc(validateFunc);
		}
	}
}

void Application::setupInfoBox()
{
	Box* dataBox = canvas->getWidget<Box>("DataBox");
	Box* infoBox = canvas->getWidget<Box>("InfoBox");
	
	////Top info buttons
	Button* button = new Button("BackButton", sf::Vector2f(110.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(), UPPER_LEFT);
	infoBox->addChildWidget(button);
	
	Label* label = new Label("BackLabel", "Back");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this, dataBox, infoBox]()
	{
		dataBox->setIsActive(true);
		infoBox->setIsActive(false);
	});
	
	//
	button = new Button("SaveButton", sf::Vector2f(110.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.5f, 0.02f), sf::Vector2f(), UP);
	button->setIsActive(false);
	infoBox->addChildWidget(button);
	
	label = new Label("SaveLabel", "Save");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this]()
	{
		std::fstream file;
		file.open("wynik.txt", std::ios::out | std::ios::trunc);
		
		auto time = std::time(nullptr);
		auto tl = *std::localtime(&time);
		file << std::put_time(&tl, "%H:%M:%S %d-%m-%Y") << " ATH Łukasz Kempys, Wojciech Dubrownik" << std::endl;
		
		file << std::endl << "Punkty:" << std::endl;
		for(unsigned int i = 0; i < examples.size(); ++i)
		{
			file << examples[i][0] << "\t" << examples[i][1] << std::endl;
		}
		
		file << std::endl << "Wagi:" << std::endl;
		std::vector<std::vector<float>> tmp = neuralNet->Weights();
		for(unsigned int i = 0; i < tmp.size(); ++i)
		{
			file << tmp[i][0] << "\t" << tmp[i][1] << "\t" << tmp[i][2] << std::endl;
		}
		
		file.close();
	});
	
	////
	button = new Button("StartButton", sf::Vector2f(110.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.97f, 0.02f), sf::Vector2f(), UPPER_RIGHT);
	infoBox->addChildWidget(button);
	
	label = new Label("StartLabel", "Start");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this]()
	{
		canvas->getWidget<Box>("HistoryBox")->setIsActive(true);
		canvas->getWidget<Box>("GeometryBox")->setIsActive(true);
		
		neuralNet->LoadWeights(weights);
		neuralNet->LoadLearningValues(examples);
		
		canvas->getWidget<GeometryGraph>("GeometryGraph")->setPoints(examples);
		
		if(slowMotion)
		{
			isLearning = true;
			canvas->getWidget<HistoryGraph>("HistoryGraph")->reset();
			canvas->getWidget<HistoryGraph>("HistoryGraph")->addWeights(neuralNet->Weights());
			canvas->getWidget<Button>("SaveButton")->setIsActive(false);
			canvas->getWidget<Button>("PredictButton")->setIsActive(false);
			canvas->getWidget<TextField>("xTextField")->setIsActive(false);
			canvas->getWidget<TextField>("yTextField")->setIsActive(false);
			canvas->getWidget<Label>("ResultLabel")->setText("");
		}
		else
		{
			neuralNet->Learn();
			canvas->getWidget<HistoryGraph>("HistoryGraph")->setWeights(neuralNet->History());
			canvas->getWidget<GeometryGraph>("GeometryGraph")->setWeights(neuralNet->Weights());
			canvas->getWidget<Button>("SaveButton")->setIsActive(true);
			canvas->getWidget<Button>("PredictButton")->setIsActive(true);
			canvas->getWidget<TextField>("xTextField")->setIsActive(true);
			canvas->getWidget<TextField>("yTextField")->setIsActive(true);
		}
	});
	
	//Predict
	button = new Button("PredictButton", sf::Vector2f(110.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.97f, 0.02f), sf::Vector2f(0.0f, 40.0f), UPPER_RIGHT);
	button->setIsActive(false);
	infoBox->addChildWidget(button);
	
	label = new Label("PredictLabel", "Predict");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this]()
	{
		std::vector<float> result = neuralNet->Predict(predictPoint);
		
		int amount = 0;
		for(unsigned int i = 0; i < result.size(); ++i)
		{
			if(result[i] == 1.0f)
			{
				++amount;
			}
		}
		
		if(amount != 1)
		{
			canvas->getWidget<Label>("ResultLabel")->setText("Nie nalezy do zadnego obszaru");
		}
		else
		{
			for(unsigned int i = 0; i < result.size(); ++i)
			{
				if(result[i] == 1.0f)
				{
					canvas->getWidget<Label>("ResultLabel")->setText("Nalezy do obszaru " + std::to_string(i + 1));
				}
			}
		}
	});
	
	label = new Label("ResultLabel", "");
	label->setAnchor(sf::Vector2f(0.97f, 0.02f), sf::Vector2f(0.0f, 160.0f), UPPER_RIGHT);
	infoBox->addChildWidget(label);
	
	TextField* textField = new TextField("xTextField", sf::Vector2f(110.0f, 30.0f));
	textField->setAnchor(sf::Vector2f(0.97f, 0.02f), sf::Vector2f(0.0f, 80.0f), UPPER_RIGHT);
	textField->setIsActive(false);
	infoBox->addChildWidget(textField);
	textField->setTextSourceFunc([this]() -> std::string
	{
		return std::to_string(predictPoint[0]);
	});
	textField->setDataUpdateFunc([this, textField]()
	{
		predictPoint[0] = atof(textField->getText().c_str());
	});
	textField->setValidateFunc([](std::string str) -> bool
	{
		return std::regex_match(str, std::regex("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
	});
	
	label = new Label("xLabel", "x");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(5.0f, 0.0f), LEFT);
	textField->addChildWidget(label);
	
	textField = new TextField("yTextField", sf::Vector2f(110.0f, 30.0f));
	textField->setAnchor(sf::Vector2f(0.97f, 0.02f), sf::Vector2f(0.0f, 120.0f), UPPER_RIGHT);
	textField->setIsActive(false);
	infoBox->addChildWidget(textField);
	textField->setTextSourceFunc([this]() -> std::string
	{
		return std::to_string(predictPoint[1]);
	});
	textField->setDataUpdateFunc([this, textField]()
	{
		predictPoint[1] = atof(textField->getText().c_str());
	});
	textField->setValidateFunc([](std::string str) -> bool
	{
		return std::regex_match(str, std::regex("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
	});
	
	label = new Label("yLabel", "y");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(5.0f, 0.0f), LEFT);
	textField->addChildWidget(label);
	
	////Text Field c
	textField = new TextField("cTextField", sf::Vector2f(80.0f, 30.0f));
	textField->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(0.0f, 40.0f), UPPER_LEFT);
	infoBox->addChildWidget(textField);
	textField->setTextSourceFunc([this]() -> std::string
	{
		return std::to_string(neuralNet->LearningRate()).substr(0, 5);
	});
	textField->setDataUpdateFunc([this, textField]()
	{
		neuralNet->LearningRate(atof(textField->getText().c_str()));
	});
	textField->setValidateFunc([](std::string str) -> bool
	{
		return std::regex_match(str, std::regex("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
	});
	
	label = new Label("cLabel", "c");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(5.0f, 0.0f), LEFT);
	textField->addChildWidget(label);
	
	//Text Field MaxIter
	textField = new TextField("MaxIterTextField", sf::Vector2f(80.0f, 30.0f));
	textField->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(0.0f, 80.0f), UPPER_LEFT);
	infoBox->addChildWidget(textField);
	textField->setTextSourceFunc([this]() -> std::string
	{
		return std::to_string(neuralNet->maxLearningIterations);
	});
	textField->setDataUpdateFunc([this, textField]()
	{
		neuralNet->maxLearningIterations = atoi(textField->getText().c_str());
	});
	textField->setValidateFunc([](std::string str) -> bool
	{
		return std::regex_match(str, std::regex("^\\d+$"));
	});
	
	label = new Label("MaxIterLabel", "Max Iterations");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(5.0f, 0.0f), LEFT);
	textField->addChildWidget(label);
	
	//Slow Button
	button = new Button("SlowButton", sf::Vector2f(120.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(0.0f, 120.0f), UPPER_LEFT);
	infoBox->addChildWidget(button);
	button->setFunc([this]()
	{
		if(isLearning && slowMotion)
		{
			isLearning = false;
			neuralNet->Learn();
			canvas->getWidget<HistoryGraph>("HistoryGraph")->setWeights(neuralNet->History());
			canvas->getWidget<GeometryGraph>("GeometryGraph")->setWeights(neuralNet->Weights());
			canvas->getWidget<Button>("SaveButton")->setIsActive(true);
			canvas->getWidget<Button>("PredictButton")->setIsActive(true);
			canvas->getWidget<TextField>("xTextField")->setIsActive(true);
			canvas->getWidget<TextField>("yTextField")->setIsActive(true);
		}
		slowMotion = !slowMotion;
	});
	
	label = new Label("SlowLabel", "Instant");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	label->setTextSourceLambda([this]() -> std::string
	{
		return slowMotion ? "Step by Step" : "Instant";
	});
	
	//Speed control
	button = new Button("SlowerButton", sf::Vector2f(30.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(0.0f, 160.0f), UPPER_LEFT);
	infoBox->addChildWidget(button);
	button->setFunc([this]()
	{
		if(speedMult > 0.15f)
		{
			speedMult /= 2.0f;
		}
	});
	
	label = new Label("SlowerLabel", "<");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	label = new Label("SpeedLabel", "1.0");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(12.5f, 0.0f), LEFT);
	button->addChildWidget(label);
	label->setTextSourceLambda([this]() -> std::string
	{
		return std::to_string(speedMult).substr(0, 3);
	});
	
	button = new Button("FasterButton", sf::Vector2f(30.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(70.0f, 160.0f), UPPER_LEFT);
	infoBox->addChildWidget(button);
	button->setFunc([this]()
	{
		speedMult *= 2.0f;
	});
	
	label = new Label("FasterLabel", ">");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	//Iter
	label = new Label("IterLabel", "0");
	label->setAnchor(sf::Vector2f(0.03, 0.02f), sf::Vector2f(0.0f, 200.0f), UPPER_LEFT);
	infoBox->addChildWidget(label);
	label->setTextSourceLambda([this]() -> std::string
	{
		return "Iteration: " + std::to_string(neuralNet->LearningIteration());
	});
	
	////Weights results
	const float offset = 28.0f;
	const float size = 80.0f;
	const float space = 8.0f;
	const int rounding = 6;
	
	for(int i = 0; i < 3; ++i)
	{
		label = new Label("W" + std::to_string(i + 1), "W" + std::to_string(i + 1));
		label->setAnchor(sf::Vector2f(0.03f, 0.7f + 0.1f * i), sf::Vector2f(), LEFT);
		infoBox->addChildWidget(label);
		
		for(int j = 0; j < 3; ++j)
		{
			label = new Label("W" + std::to_string(i + 1) + std::to_string(j + 1));
			label->setAnchor(sf::Vector2f(0.04f, 0.7f + 0.1f * i), sf::Vector2f(offset + j * size + j * space, 0.0f), LEFT);
			infoBox->addChildWidget(label);
			label->setTextSourceLambda([this, i, j, rounding]() -> std::string
			{
				return std::to_string(this->neuralNet->Weights()[i][j]).substr(0, rounding + 2);
			});
		}
	}
}

void Application::setupHistoryGraph()
{
	Box* historyBox = canvas->getWidget<Box>("HistoryBox");
	
	HistoryGraph* historyGraph = new HistoryGraph("HistoryGraph", historyBox->getSize());
	historyGraph->setAnchor(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(), UPPER_LEFT);
	historyBox->addChildWidget(historyGraph);
}

void Application::setupGeometryGraph()
{
	Box* geometryBox = canvas->getWidget<Box>("GeometryBox");
	
	GeometryGraph* geometryGraph = new GeometryGraph("GeometryGraph", geometryBox->getSize());
	geometryGraph->setAnchor(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(), UPPER_LEFT);
	geometryBox->addChildWidget(geometryGraph);
}