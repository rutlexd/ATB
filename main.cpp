#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "src/sqlite3.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

using namespace std;
using namespace sf;

struct User
{
    string name;
    string password;
    bool successfullyCheck = 0;
};

struct Goods
{
    vector <string> name;
    vector <string> count;
};

string deleteSpaceInString(string name);
static int loginCallback(void *unused, int count, char **data, char **columns);
static int listOfGoodsCallback(void *unused, int count, char **data, char **columns);
static int checkForNull_callback(void *unused, int count, char **data, char **columns);
int showLogin();
void showShop ();
bool loginUser(string inputName, string inputPassword);
Goods createGoods();
void editCount(int button, Goods goods);
Goods updateGoods();
Font getFont();
Goods editOrder(Goods order, int button, Goods goods);
Goods editOrder(Goods order, int button, Goods goods, bool minus);
string editOne(string str, bool add);
string editOne(string str, bool add, string limit);
string updateStock (vector <string> stock, int button);
vector <Text> getDataCount(Goods order);
vector <Text> getDataGood(Goods order);
vector <RectangleShape> setDataButton(Goods order);
vector <Text> setDataText(Goods order);
int stockName (string str, Goods goods);

const int WIDTH = 800;
const int HIGHT = 700;
const int FRAME = 11;
const Font defaultFont = getFont();
const char *WAYTODB = "src/Database.db";


int main(){

    showShop();

    return 0;
}

int showLogin(){
    
    RenderWindow window(VideoMode(WIDTH, HIGHT), "login");
    
    window.setFramerateLimit(FRAME);

    Color swampGreenColor = Color (62, 122, 86);
    Color coffeeColor = Color (158, 143, 103);

    RectangleShape nameInputBox (Vector2f (120 , 40));

    nameInputBox.setFillColor(Color::White);
    nameInputBox.setPosition(Vector2f (140, 150));
    nameInputBox.setOutlineThickness(5);
    nameInputBox.setOutlineColor(Color::Black);

    RectangleShape passwordInputBox (Vector2f (120, 40));

    passwordInputBox.setFillColor(Color::White);
    passwordInputBox.setPosition(Vector2f (140, 240));
    passwordInputBox.setOutlineThickness(5);
    passwordInputBox.setOutlineColor(Color::Black);

    Text nameInputText;

    nameInputText.setFont(defaultFont);
    nameInputText.setFillColor(Color::Black);
    nameInputText.setPosition(Vector2f (140, 150));
    nameInputText.setCharacterSize(14);

    Text passwordInputText;

    passwordInputText = nameInputText;
    passwordInputText.setPosition(Vector2f(140, 240));

    Text sentNameText;

    sentNameText.setString("Enter");
    sentNameText.setFont(defaultFont);
    sentNameText.setFillColor(Color::Black);
    sentNameText.setPosition(Vector2f (160, 340));

    RectangleShape sentNameButton;

    sentNameButton.setSize(Vector2f (120, 40));
    sentNameButton.setFillColor(coffeeColor);
    sentNameButton.setPosition(Vector2f (140, 340));
    sentNameButton.setOutlineThickness(5);
    sentNameButton.setOutlineColor(Color::Black);

    Text inputError;

    inputError.setFont(defaultFont);
    inputError.setFillColor(Color::Red);
    inputError.setString("Error, try again");
    inputError.setPosition(Vector2f (120, 200));
    inputError.setCharacterSize(16);

    Text nameEnter;

    nameEnter.setFont(defaultFont);
    nameEnter.setFillColor(Color::Black);
    nameEnter.setString("Name");
    nameEnter.setPosition(Vector2f (30, 150));
    nameEnter.setCharacterSize(20);

    Text passwordEnter;

    passwordEnter.setFont(defaultFont);
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
                else {
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


void showShop (){

    RenderWindow shop(VideoMode(WIDTH, HIGHT), "shop");

    shop.setFramerateLimit(FRAME);

    RectangleShape operatesButton;

    operatesButton.setSize(Vector2f (30, 30)); // Зробити створення елементів ГУІ у функції
    operatesButton.setFillColor(Color::White);
    operatesButton.setOutlineThickness(5);
    operatesButton.setOutlineColor(Color::Black);

    Text plus;

    plus.setFont(defaultFont);
    plus.setFillColor(Color::Black);
    plus.setCharacterSize(24);
    plus.setString("+");

    Text orderText = plus;

    orderText.setPosition(200, 350);
    orderText.setString("Order:");    

    int x = 20;
    int y = 20;
    
    Goods goods = createGoods(); 

    Text product;

    product.setFont(defaultFont);
    product.setFillColor(Color::Black);
    product.setCharacterSize(18);

    vector <Text> products;
    vector <Text> countProd;
    vector <Text> slesh;
    vector <Text> stockText;
    vector <string> stock;

    for (auto i = 0; i < goods.name.size(); i++){
        products.push_back(product);
        products[i].setString(goods.name[i]);
        products[i].setPosition(Vector2f (x, y));

        countProd.push_back(product);
        countProd[i].setString(goods.count[i]);
        countProd[i].setPosition(Vector2f (x + 350, y));

        stockText.push_back(product);
        stockText[i].setString(goods.count[i]);
        stockText[i].setPosition(Vector2f (x + 320, y));

        stock.push_back(goods.count[i]);

        slesh.push_back(product);
        slesh[i].setString("/");
        slesh[i].setPosition(x + 340, y);

        y += 40;
    }

    int yPls = 20;
    int xPls = 180;
   
    RectangleShape opButton;

    opButton.setSize(Vector2f(30,30));
    opButton.setOutlineThickness(3);
    opButton.setOutlineColor(Color::Black);
    opButton.setFillColor(Color::White);
   
    vector <RectangleShape> plusButton;
    vector <RectangleShape> minButton;
    vector <Text> plusText;
    vector <Text> minusText;

    for (auto i = 0; i < products.size(); i++){
        plusButton.push_back(opButton);
        plusText.push_back(plus);
        plusButton[i].setPosition(xPls, yPls);
        plusText[i].setPosition(xPls + 5, yPls);
        yPls += 40;
    }
    
    Goods order;

    vector <Text> orderGoodName;
    vector <Text> orderGoodCount;

    while (shop.isOpen()){
        Event event;
        while (shop.pollEvent(event)){
            if (event.type == Event::Closed){
                shop.close();
            }
        }

        shop.clear(Color::Cyan);
        if (Mouse::isButtonPressed(Mouse::Left)){
            Vector2i MousePos = Mouse::getPosition(shop);
            for (auto button = 0; button < products.size(); button++){
                if (plusButton[button].getGlobalBounds().contains(MousePos.x, MousePos.y)){ 
                    order = editOrder(order, button, goods);

                    stock[button] = editOne(stock[button], false, goods.count[button]);
                    stockText[button].setString(stock[button]);

                    orderGoodName = getDataGood(order);
                    orderGoodCount = getDataCount(order);

                    minButton = setDataButton(order);
                    minusText = setDataText(order);
                }
            }
            for (auto button = 0; button < order.name.size(); button++){
                if (minButton[button].getGlobalBounds().contains(MousePos.x, MousePos.y)){ 
                    int index = stockName(order.name[button], goods);
                    order = editOrder(order, button, goods, true);

                    stock[index] = editOne(stock[index], true, goods.count[index]);
                    stockText[index].setString(stock[index]);

                    orderGoodName = getDataGood(order);
                    orderGoodCount = getDataCount(order);

                    minButton = setDataButton(order);
                    minusText = setDataText(order);                    
                }
            }
        }        

        for (auto i = 0; i < products.size(); i++){
            shop.draw(products[i]);
            shop.draw(countProd[i]);

            shop.draw(slesh[i]);
            shop.draw(stockText[i]);

            shop.draw(plusButton[i]);
            shop.draw(plusText[i]);
        }

        for (auto i = 0; i < orderGoodName.size(); i++){
            shop.draw(orderGoodName[i]);
            shop.draw(orderGoodCount[i]);

            shop.draw(minButton[i]);
            shop.draw(minusText[i]);                        
        }

        shop.draw(orderText);

        shop.display();
    }
}


int stockName(string str, Goods goods){
    for (int i = 0; i < goods.name.size(); i++){
        if (str == goods.name[i]){
            return i;
        }
    }
}


vector <Text> setDataText(Goods order){
    vector <Text> output;
    Text templateText;

    templateText.setFont(defaultFont);
    templateText.setFillColor(Color::Black);
    templateText.setCharacterSize(18);
    templateText.setString("-");

    int yMin = 400;
    int xMin = 190;

    for (int i = 0; i < order.name.size(); i++){
        output.push_back(templateText);
        output[i].setPosition(xMin + 5, yMin);
        yMin += 40;
    } 

    return output;   
}


vector <RectangleShape> setDataButton(Goods order){
    vector <RectangleShape> buttons;
   
    RectangleShape opButton;

    opButton.setSize(Vector2f(30, 30));
    opButton.setOutlineThickness(3);
    opButton.setOutlineColor(Color::Black);
    opButton.setFillColor(Color::White);  

    int yMin = 400;
    int xMin = 190;

    for(int i = 0; i < order.name.size(); i++){
        buttons.push_back(opButton);
        buttons[i].setPosition(xMin, yMin);
        yMin += 40;
    }
    
    return buttons;
}


vector <Text> getDataGood(Goods order){
    Text templateText;

    templateText.setFont(defaultFont);
    templateText.setFillColor(Color::Black);
    templateText.setCharacterSize(18);

    vector <Text> output;

    int x = 20; 
    int y = 400;

    for (int i = 0; i < order.name.size(); i++){
        output.push_back(templateText);
        output[i].setString(order.name[i]);
        output[i].setPosition(x, y);
        y += 40;
    }

    return output;
}


vector <Text> getDataCount(Goods order){
    Text templateText;

    templateText.setFont(defaultFont);
    templateText.setFillColor(Color::Black);
    templateText.setCharacterSize(18);

    vector <Text> output;

    int x = 160; 
    int y = 400;

    for (int i = 0; i < order.name.size(); i++){
        output.push_back(templateText);
        output[i].setString(order.count[i]);
        output[i].setPosition(x, y);
        y += 40;
    }

    return output;
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


bool loginUser(string inputName, string inputPassword){
    string name = deleteSpaceInString(inputName);
    string password = deleteSpaceInString(inputPassword);

    sqlite3 *db;
    char *errMsg = nullptr;
    string sql = "INSERT INTO USERS(username, password) VALUES('"+ name +"', '"+ password +"');";

    int rc = sqlite3_open(WAYTODB, &db);

    User user;
    user.name = name;
    user.password = password;

    rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
    
    if (rc != SQLITE_OK ){
        sqlite3_exec(db, "SELECT * FROM USERS", loginCallback, &user, NULL);
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } 
    else {
        user.successfullyCheck = true;
        fprintf(stdout, "Records created successfully\n");
    }

    sqlite3_close(db);

    if (user.successfullyCheck){
        return true;
    }
    else {
        return false;
    }

}


static int loginCallback(void *unused, int count, char **data, char **columns){                   

    User *user = static_cast<User*>(unused);
    
    if (user->name == data[0] && user->password == data[1]){
        user -> successfullyCheck = true;
    }
    
    return 0;
}


Goods createGoods(){

    sqlite3 *db;
    Goods goods;

    sqlite3_open(WAYTODB, &db);
    sqlite3_exec(db, "SELECT * FROM GOODS", listOfGoodsCallback, &goods, NULL);
    sqlite3_close(db);

    return goods;
}


Goods editOrder(Goods order, int button, Goods goods){
    for (int i = 0; i < order.name.size(); i++){
        if (order.name[i] == goods.name[button]){
            order.count[i] = editOne(order.count[i], true, goods.count[button]);
            return order;
        }
    }

    order.name.push_back(goods.name[button]);
    order.count.push_back("1");
    
    return order;
}


Goods editOrder(Goods order, int button, Goods goods, bool minus){
    for (int i = 0; i < order.name.size(); i++){
        if (order.name[i] == order.name[button]){
            order.count[i] = editOne(order.count[i], false);
            if (order.count[i] == "0"){
                order.name.erase(order.name.begin() + i);
                order.count.erase(order.count.begin() + i);
            }
        }
    }
    return order;
}


string editOne(string str, bool add){
    int res = stoi(str);
    add ? res++ : res--;
    return res < 0 ? to_string(0) : to_string(res);
}


string editOne(string str, bool add, string limit){
    int res = stoi(str);
    add ? res++ : res--;
    res = res > stoi(limit) ? (stoi(str)) : res;
    return res < 0 ? to_string(0) : to_string(res);
}


void editCount(int button, Goods goods){

    sqlite3 *db;
    string sql;
    sqlite3_open(WAYTODB, &db);
    int miidleButton = button / 2;
    if (button % 2 == 0){ 
        sql = "UPDATE GOODS SET count = count + 1 WHERE name = '"+ goods.name[miidleButton] + "';";
    }
    else {
        if (goods.count[miidleButton] != '0'){
            sql = "UPDATE GOODS SET count = count - 1 WHERE name = '"+ goods.name[miidleButton] + "';";
        }
    }
    sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
    sqlite3_close(db);
}


Goods updateGoods(){
    Goods goods;
    sqlite3 *db;
    sqlite3_open(WAYTODB, &db);

    sqlite3_exec(db, "SELECT * FROM GOODS", listOfGoodsCallback, &goods, NULL);
    sqlite3_close(db);

    return goods;
}


static int listOfGoodsCallback(void *unused, int count, char **data, char **columns){      
  
    Goods *goods = static_cast<Goods*>(unused);
    goods -> name.push_back(data[0]);
    goods -> count.push_back(data[1]);
    
    return 0;
}


Font getFont(){ 

    Font font;
    font.loadFromFile("src/Mooli-Regular.ttf");

    return font;
}