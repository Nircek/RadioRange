#include <SFML/Graphics.hpp>
#include <cstring>

#define WINDOWWIDTH         800
#define WINDOWHEIGHT        600
#define TITLE               "Radio Range"
#define VERSIONNAME         "v0.0"
#define WINDOWFULLSCREEN    true

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOWWIDTH,WINDOWHEIGHT,32),std::string(TITLE)+" "+VERSIONNAME,WINDOWFULLSCREEN?sf::Style::Fullscreen:sf::Style::Default);
    while(window.isOpen()){
        sf::Event newEvent;
        while(window.pollEvent(newEvent)){
            if(newEvent.type==sf::Event::Closed)window.close();
            if(newEvent.type==sf::Event::KeyPressed&&newEvent.key.code==sf::Keyboard::Escape)window.close();
        }
        window.clear(sf::Color(128,128,128));
        window.display();
    }
    return 0;
}
