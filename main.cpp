#include <iostream>
#include <string>
#include <cstring>
#include "sqlite3.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

using namespace std;
using namespace sf;

string deleteSpaceInString (string name);

int main() {
    int width = 400, hight = 600;
    RenderWindow window(VideoMode(width,hight), "main");
    int frame = 11;
    window.setFramerateLimit(frame);

    Font font;
    font.loadFromFile("src/Mooli-Regular.ttf");
    if (!font.loadFromFile("src/Mooli-Regular.ttf"))
{
    cout << "Error to load font" << endl;
}

    RectangleShape nameInputBox (Vector2f (120 , 40));{

    nameInputBox.setFillColor(Color::White);
    nameInputBox.setPosition(Vector2f (140,240));
    nameInputBox.setOutlineThickness(5);
    nameInputBox.setOutlineColor(Color::Black);}

    Color color = Color (62, 122, 86, 1);


    string playerInput;
    Text playerInputText;{
    
    playerInputText.setFont(font);
    playerInputText.setFillColor(Color (0,0,0));
    playerInputText.setPosition(Vector2f (140,240));
    playerInputText.setCharacterSize(14);}


    Text sentNameText;{

    sentNameText.setString("Enter");
    sentNameText.setFont(font);
    sentNameText.setFillColor(Color (0, 0, 0));
    sentNameText.setPosition(Vector2f (160, 340));}

    RectangleShape sentNameButton;{

    sentNameButton.setSize(Vector2f (120, 40));
    sentNameButton.setFillColor(Color (158, 143, 103));
    sentNameButton.setPosition(Vector2f (140,340));
    sentNameButton.setOutlineThickness(5);
    sentNameButton.setOutlineColor(Color::Black);}


    Text nameEmpty;{

    nameEmpty.setFont(font);
    nameEmpty.setFillColor(Color::Red);
    nameEmpty.setString("Name is empty, try again");
    nameEmpty.setPosition(Vector2f (120, 200));
    nameEmpty.setCharacterSize(16);}


    bool isNameInput = false;
    bool isNameEmpty = false;

    string name = "";


    sqlite3 *db;
    char *errMsg = nullptr;
    string sql;

    while (window.isOpen()){

        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event::Closed){
                window.close();
            }
        }

    window.clear(color);

    string userEnter;

    if (event.type == Event::TextEntered && isNameInput)
    {               
        if (event.text.unicode == 8 && playerInput.length() > 0){
            playerInput.erase(playerInput.end() - 1);
            playerInputText.setString(playerInput);
        }
        else if(event.text.unicode < 128)
        {
            playerInput += event.text.unicode;
            playerInputText.setString(playerInput);
        }
    }
    
    if (Mouse::isButtonPressed(Mouse::Left)){         
        Vector2i mousePos = Mouse::getPosition(window);
        if (nameInputBox.getGlobalBounds().contains(mousePos.x, mousePos.y)){
            isNameInput = true;
        }
        else{
            isNameInput = false;
        }
        if (sentNameButton.getGlobalBounds().contains(mousePos.x, mousePos.y)){
            if (playerInput.length() > 0){
                string name = deleteSpaceInString(playerInput);
                playerInput = "";
                isNameEmpty = false;
                int rc = sqlite3_open("src/Database.db", &db);
                sql = "INSERT INTO USERS(username) VALUES('"+ name +"');";
                rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
                if( rc != SQLITE_OK ){
                    fprintf(stderr, "SQL error: %s\n", errMsg);
                    sqlite3_free(errMsg);
                } 
                else {
                    fprintf(stdout, "Records created successfully\n");
                }
                sqlite3_close(db);
            }
            else{
                cout << "name error" << endl;
                isNameEmpty = true; 
                name = "";
            }
            playerInput = "";
            playerInputText.setString(playerInput);
            
        }
    }

    window.draw(nameInputBox);
    window.draw(playerInputText);
    window.draw(sentNameButton);
    window.draw(sentNameText);

    if (isNameEmpty){
        window.draw(nameEmpty);
    }

    window.display(); 
    }

    return 0;
}

string deleteSpaceInString (string name){
    string output = "";
    for (int i = 0; i < name.length(); i++){
        if (!isspace(name[i])){
            output += name[i];
        }
    }
    return output;
}

