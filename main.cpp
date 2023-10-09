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

struct USER
{
    string name;
    string password;
    bool successfullyCheck = 0;
};

struct GOODS
{
    vector <string> name;
    vector <string> count;
};

string deleteSpaceInString (string name);
static int login_callback(void *unused, int count, char **data, char **columns);
static int listOfGoods_callback(void *unused, int count, char **data, char **columns);
static int checkForNull_callback(void *unused, int count, char **data, char **columns);
int showLogin();
void showShop ();
bool loginUser(string inputName, string inputPassword);
GOODS createListOfGoods();
void editCount(int button, GOODS goods);
GOODS updateGoods();
Font getFont();

const int WIDTH = 400;
const int HIGHT = 600;
const int FRAME = 11;
const Font FONT = getFont();
const char *WAYTODB = "src/Database.db";


int main() {

    if (showLogin() == 1){
        return 0;
    }
    showShop();

    return 0;
}

int showLogin(){
    
    RenderWindow window(VideoMode(WIDTH,HIGHT), "login");
    
    window.setFramerateLimit(FRAME);

    Color swampGreenColor = Color (62, 122, 86);  
    Color coffeColor = Color (158, 143, 103);

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
   
    nameInputText.setFont(FONT);
    nameInputText.setFillColor(Color::Black);
    nameInputText.setPosition(Vector2f (140,150));
    nameInputText.setCharacterSize(14);

    Text passwordInputText;

    passwordInputText = nameInputText;
    passwordInputText.setPosition(Vector2f(140,240));

    Text sentNameText;

    sentNameText.setString("Enter");
    sentNameText.setFont(FONT);
    sentNameText.setFillColor(Color::Black);
    sentNameText.setPosition(Vector2f (160, 340));

    RectangleShape sentNameButton;

    sentNameButton.setSize(Vector2f (120, 40));
    sentNameButton.setFillColor(coffeColor);
    sentNameButton.setPosition(Vector2f (140,340));
    sentNameButton.setOutlineThickness(5);
    sentNameButton.setOutlineColor(Color::Black);

    Text inputError;

    inputError.setFont(FONT);
    inputError.setFillColor(Color::Red);
    inputError.setString("Error, try again");
    inputError.setPosition(Vector2f (120, 200));
    inputError.setCharacterSize(16);

    Text nameEnter;

    nameEnter.setFont(FONT);
    nameEnter.setFillColor(Color::Black);
    nameEnter.setString("Name");
    nameEnter.setPosition(Vector2f (30, 150));
    nameEnter.setCharacterSize(20);

    Text passwordEnter;

    passwordEnter.setFont(FONT);
    passwordEnter.setFillColor(Color::Black);
    passwordEnter.setString("Password");
    passwordEnter.setPosition(Vector2f (30, 240));
    passwordEnter.setCharacterSize(20);

    bool isPasswordInput = false;
    bool isNameInput = false;

    bool isInputError = false;

    string playerNameInput;
    string playerPasswordInput;
   
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
                else if (event.text.unicode < 128 && event.text.unicode != 8){
                    playerPasswordInput += event.text.unicode;
                    passwordInputText.setString(playerPasswordInput);
                }
            }
        }
    
        if (Mouse::isButtonPressed(Mouse::Left)){         
            Vector2i mousePos = Mouse::getPosition(window);
            isNameInput = false;
            isPasswordInput = false;
            if (nameInputBox.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                isNameInput = true; 
            }
            else if (passwordInputBox.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                isPasswordInput = true;
            }

            if (sentNameButton.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                if (playerNameInput.length() > 0 && playerPasswordInput.length() > 0){
                    if (loginUser(playerNameInput, playerPasswordInput)){
                        window.close();
                    }
                    isInputError = false;
                }
                else{
                    isInputError = true;
                }
                
                playerNameInput = "";
                playerPasswordInput = "";
                passwordInputText.setString(playerPasswordInput);
                nameInputText.setString(playerNameInput);
                
            }
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


void showShop () {

    RenderWindow shop(VideoMode(WIDTH,HIGHT), "shop");

    shop.setFramerateLimit(FRAME);

    RectangleShape operatesButton;

    operatesButton.setSize(Vector2f (30, 30));
    operatesButton.setFillColor(Color::White);
    operatesButton.setOutlineThickness(5);
    operatesButton.setOutlineColor(Color::Black);

    Text plus;

    plus.setFont(FONT);
    plus.setFillColor(Color::Black);
    plus.setCharacterSize(24);
    plus.setString("+");

    Text minus = plus;

    minus.setString("-");
    
    int x = 20;
    int y = 20;
    
    GOODS goods = createListOfGoods(); 

    Text product;

    product.setFont(FONT);
    product.setFillColor(Color::Black);
    product.setCharacterSize(18);

    vector <Text> products;
    vector <Text> countProd;

    for (auto i = 0; i < goods.name.size(); i++){
        products.push_back(product);
        products[i].setString(goods.name[i]);
        products[i].setPosition(Vector2f (x, y));
        y += 40;
    }

    y = 20;

    for (auto i = 0; i < goods.name.size(); i++){
        countProd.push_back(product);
        countProd[i].setString(goods.count[i]);
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

        shop.clear(Color::White);

        if (Mouse::isButtonPressed(Mouse::Left)){
            Vector2i MousePos = Mouse::getPosition(shop);
            for (auto button = 0; button < opButtons.size(); button++){
                if (opButtons[button].getGlobalBounds().contains(MousePos.x, MousePos.y)){ 
                    
                    editCount(button, goods);
                    goods = updateGoods();

                    for (auto i = 0; i < goods.name.size(); i++){
                        countProd[i].setString(goods.count[i]);
                    }
                }
            }
        }        

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


bool loginUser(string inputName, string inputPassword) {
    string name = deleteSpaceInString(inputName);
    string password = deleteSpaceInString(inputPassword);

    sqlite3 *db;
    char *errMsg = nullptr;
    string sql = "INSERT INTO USERS(username, password) VALUES('"+ name +"', '"+ password +"');";

    int rc = sqlite3_open(WAYTODB, &db);

    USER user;
    user.name = name;
    user.password = password;

    rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
    
    if (rc != SQLITE_OK ){
        sqlite3_exec(db, "SELECT * FROM USERS", login_callback, &user, NULL);
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } 
    else{
        user.successfullyCheck = true;
        fprintf(stdout, "Records created successfully\n");
    }

    sqlite3_close(db);

    if (user.successfullyCheck){
        return true;
    }else{
        return false;
    }

}


static int login_callback(void *unused, int count, char **data, char **columns){                   

    USER *user = static_cast<USER*>(unused);
    
    if (user->name == data[0] && user->password == data[1]){
        user -> successfullyCheck = true;
    }
    
    return 0;
}


GOODS createListOfGoods() {

    sqlite3 *db;
    GOODS goods;

    sqlite3_open(WAYTODB, &db);
    sqlite3_exec(db, "SELECT * FROM GOODS", listOfGoods_callback, &goods, NULL);
    sqlite3_close(db);

    return goods;
}


void editCount(int button, GOODS goods){

    sqlite3 *db;
    string sql;
    sqlite3_open(WAYTODB, &db);

    if (button % 2 == 0){ 
        sql = "UPDATE GOODS SET count = count + 1 WHERE name = '"+ goods.name[button / 2] + "';";
    }
    else{
        if (goods.count[button / 2] != '0'){
            sql = "UPDATE GOODS SET count = count - 1 WHERE name = '"+ goods.name[button / 2] + "';";
        }
    }
    sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
    sqlite3_close(db);
}


GOODS updateGoods(){
    GOODS goods;
    sqlite3 *db;
    sqlite3_open(WAYTODB, &db);

    sqlite3_exec(db, "SELECT * FROM GOODS", listOfGoods_callback, &goods, NULL);
    sqlite3_close(db);

    return goods;
}
static int listOfGoods_callback(void *unused, int count, char **data, char **columns){      
  
    GOODS *goods = static_cast<GOODS*>(unused);
    goods -> name.push_back(data[0]);
    goods -> count.push_back(data[1]);
    
    return 0;
}

Font getFont(){ 

    Font font;
    font.loadFromFile("src/Mooli-Regular.ttf");

    return font;
}