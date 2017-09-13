#include <SFML/Graphics.hpp>
#include "noise/noise.h"
#include "noiseutils.h"
#include <cstring>
#include <ctime>
#include <cmath>
#include <sstream>
#include <exception>
#include <windows.h>

#include <iostream>

#define TITLE               "Radio Range"
#define VERSIONNAME         "v0.0"

#define WINDOWWIDTH         800
#define WINDOWHEIGHT        600
#define WINDOWFULLSCREEN    false

#define MAPWIDTH            600
#define MAPHEIGHT           600
#define MAPSEED             0
#define WATERLEVEL          45 // /256
#define POZIOMICALEVEL      50 // /256 czym wiecej tym wieksze poziomice

#define MAPVIEWWIDTH        600
#define MAPVIEWHEIGHT       600
#define MAPVIEWSIZE         MAPVIEWWIDTH*MAPVIEWHEIGHT
#define MAPVIEWXOFFSET      100
#define MAPVIEWYOFFSET      0
#define MAPVIEWXSCROLL      0
#define MAPVIEWYSCROLL      0

#define SIGNALMAXVALUE      500
using namespace sf;


class customException : std::exception{
    std::stringstream *ss;
    public:
    customException(){
        ss = new std::stringstream;
    }
    template <class T>
    customException operator<<(T t){
        (*ss)<<t;
        return *this;
    }
    virtual const char* what() const throw()
    {
        return ss->str().c_str();
    }

};
HWND hwnd;
void error(const char* what){
    MessageBoxA(hwnd, what, "Error", MB_ICONERROR | MB_OK);
}

typedef uint32_t pixel;
class map{
    pixel **pixels;
    const size_t mapwidth, mapheight;



    class signalslist{
        size_t length;
        map* parent;
        struct node{
            size_t id;
            class radiosignal{
                map* parent;
                double x,y;
                double alfa;
                double value;
                public:
                radiosignal(map* parent,double x,double y, double alfa, double value):
                    x(x),y(y),alfa(alfa),value(value){

                }
                bool next(){
                    x+=(cos(((alfa-90)*M_PI)/180))*2;
                    y+=(sin(((alfa-90)*M_PI)/180))*2;
                    value--;
                    return !(x<0||y<0||x>parent->mapwidth||y>parent->mapheight||value==0);
                }
                Vertex gen(size_t viewwidth, size_t viewheight,
                           size_t viewxscroll, size_t viewyscroll,
                           size_t viewxoffset, size_t viewyoffset,
                           size_t signalmaxvalue){
                    if(x>viewxscroll&&x<viewxscroll+viewwidth)return Vertex();
                    if(y>viewyscroll&&y<viewyscroll+viewheight)return Vertex();
                    uint8_t alpha;
                    if(value>signalmaxvalue)alpha=255;
                    else alpha=value/signalmaxvalue*255;return Vertex(Vector2f(x+viewxoffset,y+viewyoffset),Color(0,0,255,alpha));
                }
            } signal;
            node *next=NULL;
            node(map* parent,double x, double y, double alfa, double value):
                signal(parent,x,y,alfa,value){}
        } *begin, *end;
        public:
            signalslist(map* parent):
                length(0),parent(parent){}
        void repair(){
            node *nend=end;
            while(nend->next!=NULL)nend=nend->next;
            end=nend;
            size_t i=0;
            node* inode=begin;
            node* last=begin;
            while(inode!=NULL){
                inode->id=i;
                ++i;
                last=inode;
                inode=inode->next;
            }
            length=i;
            end=last;
        }
        void makenext(double x, double y, double alfa, double value){
            repair();
            end->next=new node(parent,x,y,alfa,value);
            end->next->id=end->id+1;
            end=end->next;
        }

    }signalslist;
    public:
    map(size_t width, size_t height, unsigned int seed=std::time(NULL)):
        mapwidth(width),mapheight(height),signalslist(this){
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
        heightMapBuilder.SetDestSize (mapwidth, mapheight);
        heightMapBuilder.SetBounds (2.0, 6.0, 1.0, 5.0);
        heightMapBuilder.Build ();

        for(size_t x=0;x<mapwidth;++x)
            for(size_t y=0;y<mapheight;++y){
                double v=0.905*heightMap.GetValue(x,y)+69;
                pixels[x][y]=v*(((pixel)1)<<(sizeof(pixel)*8-1));
            }


    }

    VertexArray genmapview(size_t viewwidth, size_t viewheight,
                           size_t viewxscroll, size_t viewyscroll,
                           size_t viewxoffset, size_t viewyoffset,
                           size_t poziomicalevel, size_t waterlevel){
        size_t x,y,size=viewwidth*viewheight;
        pixel tmp,all;

        VertexArray mapview(Points, size);
        for(size_t i=0;i<size;++i){
            x=i%viewwidth;
            y=i/viewwidth;
            tmp=(pixels[x+viewxscroll][y+viewyscroll]>>20)%256; //poziomice
            all=(pixels[x+viewxscroll][y+viewyscroll]>>(sizeof(pixel)*8-8));//przez calosc
            if(tmp>256-poziomicalevel)tmp=0;
            else tmp=255;
            if(all<waterlevel)tmp=0;
            mapview[i].position.x=x+viewxoffset;
            mapview[i].position.y=y+viewyoffset;
            mapview[i].color.a=255;
            mapview[i].color.r=tmp;
            mapview[i].color.g=tmp;
            mapview[i].color.b=(all<waterlevel)?255:0;
        }
        return mapview;
    }
    ~map(){
        for(size_t i=0;i<mapwidth;++i)
            delete pixels[i];
        delete pixels;
    }
};
using namespace noise;
int main()
{
    RenderWindow window(sf::VideoMode(WINDOWWIDTH,WINDOWHEIGHT,32),std::string(TITLE)+" "+VERSIONNAME,WINDOWFULLSCREEN?Style::Fullscreen:Style::Default);
    hwnd=window.getSystemHandle();
    try{
        //init
        map *m = new map(MAPWIDTH,MAPHEIGHT,MAPSEED);
        VertexArray mapview = m->genmapview(MAPVIEWWIDTH, MAPVIEWHEIGHT, MAPVIEWXSCROLL, MAPVIEWYSCROLL, MAPVIEWXOFFSET,MAPVIEWYOFFSET,POZIOMICALEVEL,WATERLEVEL);

        int i=10;
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
//            Vertex radiosignals[1];
//            if(i==0){
//                i=4;
//                if(!s->next()){
//                    delete s;
//                    s=new radiosignal(5,5,90+45,500);
//                }
//            }
//            radiosignals[0]=s->gen();
//            window.draw(radiosignals,1,Points);
            --i;
            //----
            window.display();
        }
        //save

        //----
        return 0;
    }catch(const std::exception &e){
        error(e.what());
    }catch(const customException &e){
        error(e.what());
    }catch(...){
        error("Unexcepted error");
    }
}
