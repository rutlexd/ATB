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
static int my_special_callback(void *unused, int count, char **data, char **columns);

string tempName = "";

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


    Text NameEnter;

    NameEnter.setFont(font);
    NameEnter.setFillColor(Color::Black);
    NameEnter.setString("Enter name");
    NameEnter.setPosition(Vector2f (120, 150));
    NameEnter.setCharacterSize(24);


    Text PasswordEnter;

    PasswordEnter.setFont(font);
    PasswordEnter.setFillColor(Color::Black);
    PasswordEnter.setString("Enter password");
    PasswordEnter.setPosition(Vector2f (120, 150));
    PasswordEnter.setCharacterSize(24);


    bool isNameEnter = true;
    bool isPasswordEnter = false;

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
                tempName = name;
                playerInput = "";
                isNameEmpty = false;

                int rc = sqlite3_open("src/Database.db", &db);
                
                sql = "SELECT username FROM USERS";
                
                sqlite3_exec(db, sql.c_str(), my_special_callback, NULL,NULL);

                sql = "INSERT INTO USERS(username, password) VALUES('"+ name +"', '123');";
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

    if (isPasswordEnter){
        window.draw(PasswordEnter);
    }
    else{
        window.draw(NameEnter);
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

static int my_special_callback(void *unused, int count, char **data, char **columns){                   

    cout << "name: "<< tempName << endl;

    if (tempName == data[0]){
        printf("%s is in table \n", data[0]);
    }
    else{
        cout<< "Isn't in table" << endl;
    }

    return 0;
}