#include <cstdio>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cstdlib>
#include <unistd.h>
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Shape.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/WindowEnums.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "hexgrid.h"
#include "SFML/System/Clock.hpp"

int main(){
    srand(time(NULL));
    sf::VideoMode screen = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode({screen.size.x, screen.size.y}), "Hex Life Automata | by Erizo", sf::Style::None);
    sf::Texture txt;
    bool notPaused = true;
    sf::IntRect rect;
    rect.left = 0;
    rect.top = 0;
    rect.width = screen.size.x;
    rect.height = screen.size.y;

    sf::Clock clock;

    sf::Sprite sprite(txt);
    sprite.setColor(sf::Color(119,58,168));
    sprite.setTextureRect(rect);

    if(!txt.create({screen.size.x,screen.size.y})){
        printf("Couldn't create Texture object.");
    }
    Hexgrid grid(txt,screen.size.x,screen.size.y, 6);
    grid.hexgridInit();

    while (window.isOpen()){
        sf::Event ev;
        while (window.pollEvent(ev)){
            switch(ev.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch(ev.key.code){
                        case sf::Keyboard::Key::Q:
                        case sf::Keyboard::Key::Escape:
                            window.close();
                            break;
                        case sf::Keyboard::Key::R:
                            grid.reset();
                            break;
                        case sf::Keyboard::Key::Space:
                            notPaused = !notPaused;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        if(clock.getElapsedTime().asMilliseconds() > 100){
            if(notPaused){
                grid.applyRules();
            }
            grid.renderGrid();
            txt.update(grid.pixels);
            grid.clear();
            clock.restart();
        }

        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }
    return 0;
}
