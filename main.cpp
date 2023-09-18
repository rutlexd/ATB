#include <iostream>
#include <string>
#include "sqlite3.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;
int main() {



    RenderWindow window(VideoMode(400,600), "main");
    
    Font font;
    font.loadFromFile("Mooli-Regular.ttf");
    if (!font.loadFromFile("Mooli-Regular.ttf"))
{
    cout << "Error to load font" << endl;
}

    

    RectangleShape button1 (Vector2f (120 , 40));


    button1.setFillColor(Color (114, 131, 204));
    button1.setPosition(Vector2f (200,125));

    RectangleShape button2 (Vector2f (120 , 40));

    button2.setFillColor(Color (114, 131, 204));
    button2.setPosition(Vector2f (200,325));

    Color color = Color (255, 255, 255, 1);

    

    while (window.isOpen()){
        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event::Closed){
                window.close();
            }
        }

    window.clear(color);
    sqlite3 *db;
    char *errMsg = nullptr;
    string userEnter;

    // Open or create a database
    int rc = sqlite3_open("Database.db", &db);


    if (Mouse::isButtonPressed(Mouse::Left)){
        Vector2i mousePos = Mouse::getPosition(window);
        if(button1.getGlobalBounds().contains(mousePos.x, mousePos.y)){
            color = Color (77, 18, 166);
            const char *sql=
          "CREATE TABLE numbers(num1 INTEGER PRIMARY KEY,num2,str);"
          "INSERT INTO numbers VALUES(1,11,'ABC');"
          "INSERT INTO numbers VALUES(2,22,'DEF');"
          "INSERT INTO numbers VALUES(3,33,'UVW');"
          "INSERT INTO numbers VALUES(4,44,'XYZ');" ;
            rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
            cout << "Button1 is presed" << endl;
        }
    }

    if (Mouse::isButtonPressed(Mouse::Left)){
        Vector2i mousePos = Mouse::getPosition(window);
        if (button2.getGlobalBounds().contains(mousePos.x, mousePos.y)){
            color = Color(62, 122, 86);
            cout << "Button1 is presed" << endl;
        }
    }


    window.draw(button1);
    window.draw(button2);




   
    sqlite3_close(db);
    window.display(); 
    }

    return 0;
}
