#include <SFML/Graphics.hpp>
#include <cstring>

#define WINDOWWIDTH         800
#define WINDOWHEIGHT        600
#define TITLE               "Radio Range"
#define VERSIONNAME         "v0.0"
#define WINDOWFULLSCREEN    true

using namespace sf;
int main()
{
    RenderWindow window(sf::VideoMode(WINDOWWIDTH,WINDOWHEIGHT,32),std::string(TITLE)+" "+VERSIONNAME,WINDOWFULLSCREEN?sf::Style::Fullscreen:sf::Style::Default);
    //init

    //----
    while(window.isOpen()){
        Event newEvent;
        while(window.pollEvent(newEvent)){
            switch(newEvent.type){
                //events

                case Event::Closed:window.close();break;

                case Event::KeyPressed:switch(newEvent.key.code){

                        case Keyboard::Escape:window.close();break;

                        default:break;

                    }break;

                default:break;
                //-----
            }
        }
        window.clear(sf::Color(128,128,128));
        //code

        //----
        window.display();
    }
    //save

    //----
    return 0;
}
