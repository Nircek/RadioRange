#include <SFML/Graphics.hpp>
#include <cstring>
#include <ctime>

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
class heightmap{
    byte **pixels;
    size_t width, height;
    public:
    heightmap(size_t width, size_t height, unsigned int seed=std::time(NULL)){
        this->width=width;
        this->height=height;
        pixels=new byte*[width];
        for(size_t i=0;i<width;++i)
            pixels[i]=new byte[height];
        std::srand(seed);
        for(size_t x=0;x<MAPWIDTH;++x)
            for(size_t y=0;y<MAPHEIGHT;++y)
                pixels[x][y]=std::rand();
    }
    VertexArray genmapview(size_t viewwidth, size_t viewheight,
                           size_t viewxscroll, size_t viewyscroll,
                           size_t viewxoffset, size_t viewyoffset){
        size_t x,y,size=viewwidth*viewheight;
        byte tmp;

        VertexArray mapview(Points, size);
        for(size_t i=0;i<size;++i){
            x=i%viewwidth;
            y=i/viewwidth;
            tmp=pixels[x+viewxscroll][y+viewyscroll];
            mapview[i].position.x=x+viewxoffset;
            mapview[i].position.y=y+viewyoffset;
            mapview[i].color.a=255;
            mapview[i].color.r=tmp;
            mapview[i].color.g=tmp;
            mapview[i].color.b=tmp;
        }
        return mapview;
    }
    ~heightmap(){
        for(size_t i=0;i<width;++i)
            delete pixels[i];
        delete pixels;
    }
};

int main()
{
    RenderWindow window(sf::VideoMode(WINDOWWIDTH,WINDOWHEIGHT,32),std::string(TITLE)+" "+VERSIONNAME,WINDOWFULLSCREEN?Style::Fullscreen:Style::Default);
    //init
    heightmap *m = new heightmap(MAPWIDTH,MAPHEIGHT,MAPSEED);
    VertexArray mapview = m->genmapview(MAPVIEWWIDTH, MAPVIEWHEIGHT, MAPVIEWXSCROLL, MAPVIEWYSCROLL, MAPVIEWXOFFSET,MAPVIEWYOFFSET);
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
