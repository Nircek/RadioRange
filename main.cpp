#include <SFML/Window.hpp>
#include <cstring>

#define WINDOWWIDTH     800
#define WINDOWHEIGHT    600
#define TITLE           "Radio Range"
#define VERSIONNAME     "v0.0"

int main()
{
    sf::Window oknoAplikacji( sf::VideoMode( WINDOWWIDTH, WINDOWHEIGHT, 32 ),  std::string(TITLE)+" "+VERSIONNAME);
    return 0;
}
