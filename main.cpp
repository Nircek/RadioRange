#include <SFML/Graphics.hpp>
#include "noise/noise.h"
#include "noiseutils.h"
#include <cstring>
#include <ctime>

#include <iostream>

#define TITLE               "Radio Range"
#define VERSIONNAME         "v0.0"

#define WINDOWWIDTH         800
#define WINDOWHEIGHT        600
#define WINDOWFULLSCREEN    false

#define MAPWIDTH            600
#define MAPHEIGHT           600
#define MAPSEED             1234
#define WATERLEVEL          45 // /256
#define POZIOMICALEVEL      50 // /256 czym wiecej tym wieksze poziomice

#define MAPVIEWWIDTH        600
#define MAPVIEWHEIGHT       600
#define MAPVIEWSIZE         MAPVIEWWIDTH*MAPVIEWHEIGHT
#define MAPVIEWXOFFSET      100
#define MAPVIEWYOFFSET      0
#define MAPVIEWXSCROLL      0
#define MAPVIEWYSCROLL      0

typedef uint32_t pixel;
using namespace sf;
class heightmap{
    pixel **pixels;
    size_t width, height;
    public:
    heightmap(size_t width, size_t height, unsigned int seed=std::time(NULL)){
        this->width=width;
        this->height=height;
        pixels=new pixel*[width];
        for(size_t i=0;i<width;++i)
            pixels[i]=new pixel[height];


        module::Perlin myModule;
        myModule.SetOctaveCount (4);
        myModule.SetFrequency (0.8);
        myModule.SetPersistence (0.3);
        myModule.SetSeed(seed);

        utils::NoiseMap heightMap;



        utils::NoiseMapBuilderPlane heightMapBuilder;
        heightMapBuilder.SetSourceModule (myModule);
        heightMapBuilder.SetDestNoiseMap (heightMap);
        heightMapBuilder.SetDestSize (MAPWIDTH, MAPHEIGHT);
        heightMapBuilder.SetBounds (2.0, 6.0, 1.0, 5.0);
        heightMapBuilder.Build ();

        for(size_t x=0;x<MAPWIDTH;++x)
            for(size_t y=0;y<MAPHEIGHT;++y){
                double v=0.905*heightMap.GetValue(x,y)+69;
                pixels[x][y]=v*(((pixel)1)<<(sizeof(pixel)*8-1));
            }


    }

    VertexArray genmapview(size_t viewwidth, size_t viewheight,
                           size_t viewxscroll, size_t viewyscroll,
                           size_t viewxoffset, size_t viewyoffset){
        size_t x,y,size=viewwidth*viewheight;
        pixel tmp,all;

        VertexArray mapview(Points, size);
        for(size_t i=0;i<size;++i){
            x=i%viewwidth;
            y=i/viewwidth;
            tmp=(pixels[x+viewxscroll][y+viewyscroll]>>20)%256; //poziomice
            all=(pixels[x+viewxscroll][y+viewyscroll]>>(sizeof(pixel)*8-8));//przez calosc
            if(tmp>256-POZIOMICALEVEL)tmp=0;
            else tmp=255;
            if(all<WATERLEVEL)tmp=0;
            mapview[i].position.x=x+viewxoffset;
            mapview[i].position.y=y+viewyoffset;
            mapview[i].color.a=255;
            mapview[i].color.r=tmp;
            mapview[i].color.g=tmp;
            mapview[i].color.b=(all<WATERLEVEL)?255:0;
        }
        return mapview;
    }
    ~heightmap(){
        for(size_t i=0;i<width;++i)
            delete pixels[i];
        delete pixels;
    }
};
using namespace noise;
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
