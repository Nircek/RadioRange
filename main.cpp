#include <SFML/Graphics.hpp>
#include <cstring>

#define TITLE               "Radio Range"
#define VERSIONNAME         "v0.0"

#define WINDOWWIDTH         800
#define WINDOWHEIGHT        600
#define WINDOWFULLSCREEN    false

#define MAPWIDTH            600
#define MAPHEIGHT           600
#define MAPSEED             1234

#define MAPVIEWWIDTH        600
#define MAPVIEWHEIGHT       600
#define MAPVIEWSIZE         MAPVIEWWIDTH*MAPVIEWHEIGHT
#define MAPVIEWXOFFSET      100
#define MAPVIEWYOFFSET      0
#define MAPVIEWXSCROLL      0
#define MAPVIEWYSCROLL      0

typedef uint8_t byte;
using namespace sf;
int main()
{
    RenderWindow window(sf::VideoMode(WINDOWWIDTH,WINDOWHEIGHT,32),std::string(TITLE)+" "+VERSIONNAME,WINDOWFULLSCREEN?Style::Fullscreen:Style::Default);
    //init
    byte map[MAPWIDTH][MAPHEIGHT];
    size_t x,y,i;
    byte tmp;
    std::srand(MAPSEED);
    for(x=0;x<MAPWIDTH;++x)
        for(y=0;y<MAPHEIGHT;++y)
            map[x][y]=std::rand();
    sf::VertexArray mapview(sf::Points, MAPVIEWSIZE);
    for(i=0;i<MAPVIEWSIZE;++i){
        x=i%MAPWIDTH;
        y=i/MAPWIDTH;
        tmp=map[x+MAPVIEWXSCROLL][y+MAPVIEWYSCROLL];
        mapview[i].position.x=x+MAPVIEWXOFFSET;
        mapview[i].position.y=y+MAPVIEWYOFFSET;
        mapview[i].color.a=255;
        mapview[i].color.r=tmp;
        mapview[i].color.g=tmp;
        mapview[i].color.b=tmp;
    }
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
        window.draw(mapview);

        //----
        window.display();
    }
    //save

    //----
    return 0;
}
