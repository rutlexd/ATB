#include <iostream>
#include <string>
#include "sqlite3.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

using namespace std;
using namespace sf;
int main() {



    RenderWindow window(VideoMode(400,600), "main");
    
    window.setFramerateLimit(12);

    Font font;
    font.loadFromFile("Mooli-Regular.ttf");
    if (!font.loadFromFile("Mooli-Regular.ttf"))
{
    cout << "Error to load font" << endl;
}

    


    RectangleShape nameInputBox (Vector2f (120 , 40));

    nameInputBox.setFillColor(Color (255, 255, 255));
    nameInputBox.setPosition(Vector2f (140,240));

    nameInputBox.setOutlineThickness(5);
    nameInputBox.setOutlineColor(Color (0, 0, 0));


    Color color = Color (62, 122, 86, 1);

    string playerInput;
    string temp = "";
    Text playerText;
    
    playerText.setFont(font);
    playerText.setFillColor(Color (0,0,0));
    playerText.setPosition(Vector2f (140,240));
    playerText.setCharacterSize(14);

    bool isNameInput = false;


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


    if (event.type == Event::TextEntered && isNameInput)
    {               
        if (event.text.unicode == 8 && playerInput.length() > 0){
            playerInput.erase(playerInput.end() - 1);
            playerText.setString(playerInput);
        }
        else if(event.text.unicode < 128)
        {
            playerInput += event.text.unicode;
            temp += playerInput;
            int test = event.text.unicode;
            playerText.setString(playerInput);
            cerr << temp << endl;
            cout << test << " ----" << endl;
        }
    }
    if (Mouse::isButtonPressed(Mouse::Left)){           // Check for click in button
        Vector2i mousePos = Mouse::getPosition(window);
        if (nameInputBox.getGlobalBounds().contains(mousePos.x, mousePos.y)){
            
            isNameInput = true;

            cout << "Button2 is presed" << endl;
        }
        else{
            isNameInput = false;
        }
    }

    

    window.draw(nameInputBox);
    window.draw(playerText);


    // sqlite3_close(db);
    window.display(); 
    }

    
    return 0;
}
