#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

using namespace sf;

Font getFont(){ 

    Font font;
    font.loadFromFile("src/Mooli-Regular.ttf");

    return font;
}

const Color swampGreenColor = Color (62, 122, 86);
const Color coffeeColor = Color (158, 143, 103);
const int WIDTH = 800;
const int HIGHT = 700;
const int FRAME = 11;
const int ROW_HIGHT = 40;
const Font DEFAULT_FONT = getFont();