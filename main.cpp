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



    RenderWindow window(VideoMode(400,600), "main");
    
    window.setFramerateLimit(10);

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
    Text playerText;
    
    playerText.setFont(font);
    playerText.setFillColor(Color (0,0,0));
    playerText.setPosition(Vector2f (140,240));
    playerText.setCharacterSize(14);


    Text sentNameText;

    sentNameText.setString("Enter");
    sentNameText.setFont(font);
    sentNameText.setFillColor(Color (0, 0, 0));
    sentNameText.setPosition(Vector2f (160, 340));

    RectangleShape sentNameButton;

    sentNameButton.setSize(Vector2f (120, 40));
    sentNameButton.setFillColor(Color (158, 143, 103));
    sentNameButton.setPosition(Vector2f (140,340));
    sentNameButton.setOutlineThickness(5);
    sentNameButton.setOutlineColor(Color (0, 0, 0));


    Text nameEmpty;

    nameEmpty.setFont(font);
    nameEmpty.setFillColor(Color (224, 43, 70));
    nameEmpty.setString("Name is empty, try again");
    nameEmpty.setPosition(Vector2f (120, 200));
    nameEmpty.setCharacterSize(16);


    bool isNameInput = false;
    bool isNameEmpty = false;

    string name = "";


    sqlite3 *db;
    char *errMsg = nullptr;
    char *sql;

    while (window.isOpen()){

        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event::Closed){
                window.close();
            }
        }

    window.clear(color);

    string userEnter;


    // Open or create a database
    

    if (event.type == Event::TextEntered && isNameInput)
    {               
        if (event.text.unicode == 8 && playerInput.length() > 0){
            playerInput.erase(playerInput.end() - 1);
            playerText.setString(playerInput);
        }
        else if(event.text.unicode < 128)
        {
            playerInput += event.text.unicode;
            playerText.setString(playerInput);
        }
    }
    
    if (Mouse::isButtonPressed(Mouse::Left)){           // Check for click in button
        Vector2i mousePos = Mouse::getPosition(window);
        if (nameInputBox.getGlobalBounds().contains(mousePos.x, mousePos.y)){
            isNameInput = true;
        }
        else{
            isNameInput = false;
        }
        if (sentNameButton.getGlobalBounds().contains(mousePos.x, mousePos.y)){
            if (playerInput.length() > 0){
                name = deleteSpaceInString(playerInput);
                playerInput = "";
                isNameEmpty = false;
                int rc = sqlite3_open("Database.db", &db);

                const char* str = name.c_str();

                string sqlQuery = "INSERT INTO USERS (userame, password) VALUES (?, ?)";        
                sqlite3_stmt *stmt;

                rc = sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt, nullptr);
                if( rc != SQLITE_OK ){
                    fprintf(stderr, "SQL error: %s\n", errMsg);
                    sqlite3_free(errMsg);
                } 
                else {
                    fprintf(stdout, "Records created successfully\n");
                }
                sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
                int password = 1432;
                sqlite3_bind_int(stmt, 2, password);
                rc = sqlite3_step(stmt);
                sqlite3_finalize(stmt);
                sqlite3_close(db);
            }
            else{
                cout << "name error" << endl;
                isNameEmpty = true; 
                name = "";
            }
            playerInput = "";
            playerText.setString(playerInput);
            
        }
    }

    

    

    window.draw(nameInputBox);
    window.draw(playerText);
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