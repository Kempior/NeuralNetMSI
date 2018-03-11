#include <SFML/Graphics.hpp>

#include "ServiceLocator/Locator.hpp"
#include "ServiceLocator/DefaultResourceFont.hpp"

#include "Button.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Neural Net");
    
	Locator::provideFont(new DefaultResourceFont());
	
	bool bColor = false;
	
    Button b(sf::Vector2f(20.0f, 20.0f), sf::Vector2f(100.0f, 40.0f), "Button");
	b.setFunc([&b, &bColor](){
		if(bColor)
		{
			b.setColor(sf::Color::Blue);
		}
		else
		{
			b.setColor(sf::Color::Red);
		}
		bColor = !bColor;
	});
	
	b.setColor(sf::Color::Blue);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
			
			b.handleEvent(event);
        }

        window.clear();
		b.draw(window);
        window.display();
    }

	Locator::terminate();

    return 0;
}