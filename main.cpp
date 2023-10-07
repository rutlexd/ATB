#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "sqlite3.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

using namespace std;
using namespace sf;

string deleteSpaceInString (string name);
static int login_callback(void *unused, int count, char **data, char **columns);
static int listOfGoods_callback(void *unused, int count, char **data, char **columns);
static int checkForNull_callback(void *unused, int count, char **data, char **columns);
int showLogin(Font font);
void showShop (Font font);

string globalName = "";
string globalPassword = "";

bool loginSucceffuly = false;

vector <string> prod; 
vector <string> prodCount;

string GlobalCount;

const int WIDTH = 400;
const int HIGHT = 600;
const int FRAME = 11;

int main() {

    Font font;
    font.loadFromFile("src/Mooli-Regular.ttf");
    if (!font.loadFromFile("src/Mooli-Regular.ttf")){
    cout << "Error to load font" << endl;
    }
    int CheckForCLose = showLogin(font);
    if (CheckForCLose == 1){
        return 0;
    }
    showShop(font);

    return 0;
}

int showLogin(Font font){
    
    RenderWindow window(VideoMode(WIDTH,HIGHT), "login");
    
    window.setFramerateLimit(FRAME);

    Color swampGreenColor = Color (62, 122, 86, 1);  

    RectangleShape nameInputBox (Vector2f (120 , 40));

    nameInputBox.setFillColor(Color::White);
    nameInputBox.setPosition(Vector2f (140,150));
    nameInputBox.setOutlineThickness(5);
    nameInputBox.setOutlineColor(Color::Black);

    RectangleShape passwordInputBox (Vector2f (120 , 40));

    passwordInputBox.setFillColor(Color::White);
    passwordInputBox.setPosition(Vector2f (140,240));
    passwordInputBox.setOutlineThickness(5);
    passwordInputBox.setOutlineColor(Color::Black);

    Text nameInputText;
    
    nameInputText.setFont(font);
    nameInputText.setFillColor(Color::Black);
    nameInputText.setPosition(Vector2f (140,150));
    nameInputText.setCharacterSize(14);

    Text passwordInputText;

    passwordInputText = nameInputText;
    passwordInputText.setPosition(Vector2f(140,240));

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
    sentNameButton.setOutlineColor(Color::Black);

    Text inputError;

    inputError.setFont(font);
    inputError.setFillColor(Color::Red);
    inputError.setString("Error, try again");
    inputError.setPosition(Vector2f (120, 200));
    inputError.setCharacterSize(16);

    Text nameEnter;

    nameEnter.setFont(font);
    nameEnter.setFillColor(Color::Black);
    nameEnter.setString("Name");
    nameEnter.setPosition(Vector2f (30, 150));
    nameEnter.setCharacterSize(20);

    Text passwordEnter;

    passwordEnter.setFont(font);
    passwordEnter.setFillColor(Color::Black);
    passwordEnter.setString("Password");
    passwordEnter.setPosition(Vector2f (30, 240));
    passwordEnter.setCharacterSize(20);

    bool isPasswordInput = false;
    bool isNameInput = false;

    bool isInputError = false;

    string playerNameInput;
    string playerPasswordInput;
    string name = "";
   
    sqlite3 *db;
    char *errMsg = nullptr;
    string sql;

    while (window.isOpen()){

        Event event;
        while (window.pollEvent(event)){
            if (event.type == Event::Closed){
                window.close();
                return 1;
            }
        }

        window.clear(swampGreenColor);

        if (event.type == Event::TextEntered){   
            if (isNameInput){            
                if (event.text.unicode == 8 && playerNameInput.length() > 0){
                    playerNameInput.erase(playerNameInput.end() - 1);
                    nameInputText.setString(playerNameInput);
                }
                else if (event.text.unicode < 128 && event.text.unicode != 8)
                {
                    playerNameInput += event.text.unicode;
                    nameInputText.setString(playerNameInput);
                }
            }
            else if (isPasswordInput){
                if (event.text.unicode == 8 && playerPasswordInput.length() > 0){
                    playerPasswordInput.erase(playerPasswordInput.end() - 1);
                    passwordInputText.setString(playerPasswordInput);
                }
                else if (event.text.unicode < 128 && event.text.unicode != 8)
                {
                    playerPasswordInput += event.text.unicode;
                    passwordInputText.setString(playerPasswordInput);
                }
            }
        }
    
        if (Mouse::isButtonPressed(Mouse::Left)){         
            Vector2i mousePos = Mouse::getPosition(window);
            isNameInput = false;
            if (nameInputBox.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                isNameInput = true;
                isPasswordInput = false;
            }
            else if (passwordInputBox.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                isPasswordInput = true;
                isNameInput = false;
            }

            if (sentNameButton.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                if (playerNameInput.length() > 0 && playerPasswordInput.length() > 0){
                    string name = deleteSpaceInString(playerNameInput);
                    string password = deleteSpaceInString(playerPasswordInput);
                    globalName = name;
                    globalPassword = password;
                    playerNameInput = "";
                    isInputError = false;

                    int rc = sqlite3_open("src/Database.db", &db);
                    
                    sql = "SELECT * FROM USERS";
                    
                    sqlite3_exec(db, sql.c_str(), login_callback, NULL,NULL);
                    sql = "INSERT INTO USERS(username, password) VALUES('"+ name +"', '"+ password +"');";
                    rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
                    
                    if( rc != SQLITE_OK ){
                        fprintf(stderr, "SQL error: %s\n", errMsg);
                        sqlite3_free(errMsg);
                    } 
                    else {
                        loginSucceffuly = true;
                        fprintf(stdout, "Records created successfully\n");
                    }

                
                    sqlite3_close(db);
                }
                else{
                    cout << "name error" << endl;
                    isInputError = true; 
                    name = "";
                }
                
                playerNameInput = "";
                playerPasswordInput = "";
                passwordInputText.setString(playerPasswordInput);
                nameInputText.setString(playerNameInput);
                
            }
        }

        if (loginSucceffuly){
            window.close();
        }


        window.draw(nameInputBox);
        window.draw(passwordInputBox);
        window.draw(passwordEnter);
        window.draw(nameEnter);

        window.draw(passwordInputText);
        window.draw(nameInputText);

        window.draw(sentNameButton);
        window.draw(sentNameText);   
        if (isInputError){
            window.draw(inputError);
        }
        window.display(); 
    }
    return 0;
}


void showShop (Font font) {

    RenderWindow shop(VideoMode(WIDTH,HIGHT), "shop");

    shop.setFramerateLimit(FRAME);

    sqlite3 *db;
    string sql;

    RectangleShape operatesButton;
    operatesButton.setSize(Vector2f (30, 30));
    operatesButton.setFillColor(Color::White);
    operatesButton.setOutlineThickness(5);
    operatesButton.setOutlineColor(Color::Black);

    Text plus;
    plus.setFont(font);
    plus.setFillColor(Color::Black);
    plus.setCharacterSize(24);
    plus.setString("+");

    Text minus = plus;
    minus.setString("-");

    sqlite3_open("src/Database.db", &db);  

    sql = "SELECT * FROM GOODS;";

    sqlite3_exec(db, sql.c_str(), listOfGoods_callback, NULL, NULL); // func create vector of GOODS

    sqlite3_close(db);
    
    Text product;
    int x = 20;
    int y = 20;

    product.setFont(font);
    product.setFillColor(Color::Black);
    product.setCharacterSize(18);

    vector <Text> products;
    vector <Text> countProd;

    for (auto i = 0; i < prod.size(); i++){
        products.push_back(product);
        products[i].setString(prod[i]);
        products[i].setPosition(Vector2f (x, y));
        y += 40;
    }

    y = 20;

    for (auto i = 0; i < prod.size(); i++){
        countProd.push_back(product);
        countProd[i].setString(prodCount[i]);
        countProd[i].setPosition(Vector2f (x + 350, y));
        y += 40;
    }

    int yPls = 20;
    int yMin = 20;
    x = 150;

    RectangleShape opButton;

    opButton.setSize(Vector2f(30,30));
    opButton.setOutlineThickness(3);
    opButton.setOutlineColor(Color::Black);
    opButton.setFillColor(Color::White);
    
    vector <RectangleShape> opButtons;
    vector <Text> opText;
    
    for (auto i = 0; i < products.size() * 2; i++){
        opButtons.push_back(opButton);
        if (i % 2 == 0){
            opText.push_back(plus);
            opButtons[i].setPosition(x, yPls);
            opText[i].setPosition(x + 5, yPls);
            yPls += 40;
        }
        else if (i % 2 != 0){
            opText.push_back(minus);
            opButtons[i].setPosition(x + 50, yMin);
            opText[i].setPosition(x + 55, yMin);
            yMin += 40;
        }
    }

    while (shop.isOpen()){
        Event event;
        while (shop.pollEvent(event)){
            if (event.type == Event::Closed){
                shop.close();
            }
        }

        sqlite3_open("src/Database.db", &db);
        shop.clear(Color::White);

        if (Mouse::isButtonPressed(Mouse::Left)){
            Vector2i MousePos = Mouse::getPosition(shop);
            for (auto i = 0; i < opButtons.size(); i++){
                if (opButtons[i].getGlobalBounds().contains(MousePos.x, MousePos.y)){ 
                    cout << i << endl;
                    if (i % 2 == 0){ 
                        sql = "UPDATE GOODS SET count = count + 1 WHERE name = '"+ prod[i / 2] + "';";
                    }
                    else{
                        sql = "SELECT count FROM GOODS WHERE name = '"+ prod[i / 2] +"';";
                        sqlite3_exec(db, sql.c_str(), checkForNull_callback, NULL, NULL);
                        if (GlobalCount != '0'){
                            sql = "UPDATE GOODS SET count = count - 1 WHERE name = '"+ prod[i / 2] + "';";
                        }
                    }
                    sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
                    prodCount = {};
                    sql = "SELECT * FROM GOODS;";
                    
                    sqlite3_exec(db, sql.c_str(), listOfGoods_callback, NULL, NULL);

                    for (auto i = 0; i < products.size(); i++){
                        countProd[i].setString(prodCount[i]);
                    }
                }
            }
        }

        sqlite3_close(db);
        for (auto i = 0; i < products.size(); i++){
            shop.draw(products[i]);
            shop.draw(countProd[i]);
        }

        for (auto i = 0; i < products.size() * 2; i++){
            shop.draw(opButtons[i]);
            shop.draw(opText[i]);
        }

        shop.display();
    }
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

    cout << "name: " << globalName << endl;
    cout << "password " << globalPassword << endl;
    if (globalName == data[0]){
        printf("%s is in table \n", data[0]);
        printf("%s is password \n", data[1]);
        if (globalPassword == data[1]){
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
    prod.push_back(data[0]);
    prodCount.push_back(data[1]);

    return 0;
}
static int checkForNull_callback(void *unused, int count, char **data, char **columns){      
    GlobalCount = data[0];
    return 0;
}