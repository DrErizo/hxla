#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>

int main(){
    // create the window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");

    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.display();
    }

    return 0;
}
