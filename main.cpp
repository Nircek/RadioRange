#include <SFML/Window.hpp>
#include <cstring>

#define WINDOWWIDTH         800
#define WINDOWHEIGHT        600
#define TITLE               "Radio Range"
#define VERSIONNAME         "v0.0"
#define WINDOWFULLSCREEN    false

int main()
{
    sf::Window window( sf::VideoMode( WINDOWWIDTH, WINDOWHEIGHT, 32 ),  std::string(TITLE)+" "+VERSIONNAME, WINDOWFULLSCREEN?sf::Style::Fullscreen:sf::Style::Default);
    while(window.isOpen()){
        sf::Event newEvent;
        while( window.pollEvent( newEvent ) )
        {
            if(newEvent.type==sf::Event::Closed){
                window.close();
            }
        }
        window.display();
    }
    return 0;
}
