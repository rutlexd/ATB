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
static int login_callback(void *unused, int count, char **data, char **columns);
static int listOfGoods_callback(void *unused, int count, char **data, char **columns);

string tempName = "";
string tempPassword = "";

bool loginSucceffuly = false;

string prod; 

int main() {
    int width = 400, hight = 600;
    // RenderWindow window(VideoMode(width,hight), "main");
    int frame = 11;
    // window.setFramerateLimit(frame);

    Font font;
    font.loadFromFile("src/Mooli-Regular.ttf");
    if (!font.loadFromFile("src/Mooli-Regular.ttf"))
    {
    cout << "Error to load font" << endl;
    }

    Color color = Color (62, 122, 86, 1);  // Колір ригачів

    RectangleShape nameInputBox (Vector2f (120 , 40));{

    nameInputBox.setFillColor(Color::White);
    nameInputBox.setPosition(Vector2f (140,150));
    nameInputBox.setOutlineThickness(5);
    nameInputBox.setOutlineColor(Color::Black);}

    RectangleShape passwordInputBox (Vector2f (120 , 40));{

    passwordInputBox.setFillColor(Color::White);
    passwordInputBox.setPosition(Vector2f (140,240));
    passwordInputBox.setOutlineThickness(5);
    passwordInputBox.setOutlineColor(Color::Black);}


    Text nameInputText;{
    
    nameInputText.setFont(font);
    nameInputText.setFillColor(Color::Black);
    nameInputText.setPosition(Vector2f (140,150));
    nameInputText.setCharacterSize(14);}

    Text passwordInputText = nameInputText;

    passwordInputText.setPosition(Vector2f(140,240));


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


    Text inputError;{

    inputError.setFont(font);
    inputError.setFillColor(Color::Red);
    inputError.setString("Error, try again");
    inputError.setPosition(Vector2f (120, 200));
    inputError.setCharacterSize(16);}


    Text NameEnter;

    NameEnter.setFont(font);
    NameEnter.setFillColor(Color::Black);
    NameEnter.setString("Name");
    NameEnter.setPosition(Vector2f (30, 150));
    NameEnter.setCharacterSize(20);


    Text PasswordEnter;

    PasswordEnter.setFont(font);
    PasswordEnter.setFillColor(Color::Black);
    PasswordEnter.setString("Password");
    PasswordEnter.setPosition(Vector2f (30, 240));
    PasswordEnter.setCharacterSize(20);


    bool isPasswordInput = false;
    bool isNameInput = false;

    bool isInputError = false;

    string playerNameInput;
    string playerPasswordInput;
    string name = "";
   
    sqlite3 *db;
    char *errMsg = nullptr;
    string sql;


//     while (window.isOpen()){

//         Event event;
//         while(window.pollEvent(event)){
//             if(event.type == Event::Closed){
//                 window.close();
//                 return 0;
//             }
//         }

//     window.clear(color);
// \
//     if (event.type == Event::TextEntered)
//     {   
//         if(isNameInput){            
//             if (event.text.unicode == 8 && playerNameInput.length() > 0){
//                 playerNameInput.erase(playerNameInput.end() - 1);
//                 nameInputText.setString(playerNameInput);
//             }
//             else if(event.text.unicode < 128)
//             {
//                 playerNameInput += event.text.unicode;
//                 nameInputText.setString(playerNameInput);
//             }
//         }
//         else if (isPasswordInput){
//             if (event.text.unicode == 8 && playerPasswordInput.length() > 0){
//                 playerPasswordInput.erase(playerPasswordInput.end() - 1);
//                 passwordInputText.setString(playerPasswordInput);
//             }
//             else if(event.text.unicode < 128)
//             {
//                 playerPasswordInput += event.text.unicode;
//                 passwordInputText.setString(playerPasswordInput);
//             }
//         }
//     }
    
//     if (Mouse::isButtonPressed(Mouse::Left)){         
//         Vector2i mousePos = Mouse::getPosition(window);
//         if (nameInputBox.getGlobalBounds().contains(mousePos.x, mousePos.y)){
//             isNameInput = true;
//             isPasswordInput = false;
//         }
//         else if (passwordInputBox.getGlobalBounds().contains(mousePos.x, mousePos.y)){
//             isPasswordInput = true;
//             isNameInput = false;
//         }
//         else{
//             isNameInput = false;
//         }
//         if (sentNameButton.getGlobalBounds().contains(mousePos.x, mousePos.y)){
//             if (playerNameInput.length() > 0 && playerPasswordInput.length() > 0){
//                 string name = deleteSpaceInString(playerNameInput);
//                 string password = deleteSpaceInString(playerPasswordInput);
//                 tempName = name;
//                 tempPassword = password;
//                 playerNameInput = "";
//                 isInputError = false;

//                 int rc = sqlite3_open("src/Database.db", &db);
                
//                 sql = "SELECT * FROM USERS";
                
//                 sqlite3_exec(db, sql.c_str(), login_callback, NULL,NULL);
//                 sql = "INSERT INTO USERS(username, password) VALUES('"+ name +"', '"+ password +"');";
//                 rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
                
//                 if( rc != SQLITE_OK ){
//                     fprintf(stderr, "SQL error: %s\n", errMsg);
//                     sqlite3_free(errMsg);
//                 } 
//                 else {
//                     loginSucceffuly = true;
//                     fprintf(stdout, "Records created successfully\n");
//                 }

            
//                 sqlite3_close(db);
//             }
//             else{
//                 cout << "name error" << endl;
//                 isInputError = true; 
//                 name = "";
//             }
            
//             playerNameInput = "";
//             playerPasswordInput = "";
//             passwordInputText.setString(playerPasswordInput);
//             nameInputText.setString(playerNameInput);
            
//         }
//     }

//     if (loginSucceffuly){
//         window.close();
//     }


//     window.draw(nameInputBox);
//     window.draw(passwordInputBox);
//     window.draw(PasswordEnter);
//     window.draw(NameEnter);

//     window.draw(passwordInputText);
//     window.draw(nameInputText);

//     window.draw(sentNameButton);
//     window.draw(sentNameText);   
//     if (isInputError){
//         window.draw(inputError);
//     }
//     window.display(); 
//     }

    RenderWindow shop(VideoMode(width,hight), "shop");

    shop.setFramerateLimit(frame);

    RectangleShape operatesButton;
    operatesButton.setSize(Vector2f (30, 30));
    operatesButton.setFillColor(Color::White);
    operatesButton.setOutlineThickness(5);
    operatesButton.setOutlineColor(Color::Black);

    Text plus;
    plus.setFont(font);
    plus.setFillColor(Color::Black);
    plus.setCharacterSize(12);
    plus.setString("+");

    
    Text product;

    product.setFont(font);
    product.setFillColor(Color::Black);
    product.setCharacterSize(12);
    product.setPosition(Vector2f(20, 20));

    

    while (shop.isOpen()){
        Event event;
        while(shop.pollEvent(event)){
            if(event.type == Event::Closed){
                shop.close();
            }
    }
    char* sqlt;
    int secondTable = sqlite3_open("src/Database.db", &db);  // Винести з циклу та занести в список, отримувати лише один раз
                                                            // Тут занести каунт щоб постійно обновлювалась та реалізувати видалляна даних

    sqlt = "SELECT name FROM GOODS";

    sqlite3_exec(db, sqlt, listOfGoods_callback, NULL, NULL);
    product.setString(prod);
    sqlite3_close(db);
    shop.clear(Color::White);


    shop.draw(product);

    shop.display();
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

static int login_callback(void *unused, int count, char **data, char **columns){                   

    cout << "name: " << tempName << endl;
    cout << "password " << tempPassword << endl;
    if (tempName == data[0]){
        printf("%s is in table \n", data[0]);
        printf("%s is password \n", data[1]);
        if (tempPassword == data[1]){
            cout << "login succeffuly" << endl;
            loginSucceffuly = true;
        }
        else{
            cout << "Inccorect password" << endl;
        }
    }

    return 0;
}



static int listOfGoods_callback(void *unused, int count, char **data, char **columns){      
    cout << data[0] << endl;
    prod = data[0];

    return 0;
}
