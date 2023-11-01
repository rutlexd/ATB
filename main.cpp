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

struct Good
{
    string name;
    int count;
};

struct Order
{
    string goodName;
    int count;
    User user;
};

string deleteSpaceInString(string name);
static int loginCallback(void *unused, int count, char **data, char **columns);
static int listOfGoodsCallback(void *unused, int count, char **data, char **columns);
static int checkForNull_callback(void *unused, int count, char **data, char **columns);
bool checkLogin();
void showShop ();
bool loginUser(string inputName, string inputPassword);
vector <Good> createGoods();
void editCount(int button, vector <Good> goods);
vector <Good> updateGoods();
Font getFont();
vector <Order> editOrder(vector <Order> orders, int button, vector <Good> goods);
vector <Order> editOrder(vector <Order> orders, int button, vector <Good> goods, bool minus);
int editOne(int num, bool add, int limit);
string updateStock (vector <string> stock, int button);
vector <Text> getDataCount(vector <Order> orders);
vector <Text> getDataGood(vector <Order> orders);
vector <RectangleShape> setDataButton(vector <Order> orders);
vector <Text> setDataText(vector <Order> orders);
int stockName (string str, vector <Good> goods);

const int WIDTH = 800;
const int HIGHT = 700;
const int FRAME = 11;
const Font defaultFont = getFont();
const char *WAYTODB = "src/Database.db";


int main(){

    showShop();

    return 0;
}


// bool checkLogin(){
    
//     RenderWindow window(VideoMode(WIDTH, HIGHT), "login");
    
//     window.setFramerateLimit(FRAME);

//     Color swampGreenColor = Color (62, 122, 86);
//     Color coffeeColor = Color (158, 143, 103);

//     RectangleShape nameInputBox (Vector2f (120 , 40));

//     nameInputBox.setFillColor(Color::White);
//     nameInputBox.setPosition(Vector2f (140, 150));
//     nameInputBox.setOutlineThickness(5);
//     nameInputBox.setOutlineColor(Color::Black);

//     RectangleShape passwordInputBox (Vector2f (120, 40));

//     passwordInputBox.setFillColor(Color::White);
//     passwordInputBox.setPosition(Vector2f (140, 240));
//     passwordInputBox.setOutlineThickness(5);
//     passwordInputBox.setOutlineColor(Color::Black);

//     Text nameInputText;

//     nameInputText.setFont(defaultFont);
//     nameInputText.setFillColor(Color::Black);
//     nameInputText.setPosition(Vector2f (140, 150));
//     nameInputText.setCharacterSize(14);

//     Text passwordInputText;

//     passwordInputText = nameInputText;
//     passwordInputText.setPosition(Vector2f(140, 240));

//     Text sentNameText;

//     sentNameText.setString("Enter");
//     sentNameText.setFont(defaultFont);
//     sentNameText.setFillColor(Color::Black);
//     sentNameText.setPosition(Vector2f (160, 340));

//     RectangleShape sentNameButton;

//     sentNameButton.setSize(Vector2f (120, 40));
//     sentNameButton.setFillColor(coffeeColor);
//     sentNameButton.setPosition(Vector2f (140, 340));
//     sentNameButton.setOutlineThickness(5);
//     sentNameButton.setOutlineColor(Color::Black);

//     Text inputError;

//     inputError.setFont(defaultFont);
//     inputError.setFillColor(Color::Red);
//     inputError.setString("Error, try again");
//     inputError.setPosition(Vector2f (120, 200));
//     inputError.setCharacterSize(16);

//     Text nameEnter;

//     nameEnter.setFont(defaultFont);
//     nameEnter.setFillColor(Color::Black);
//     nameEnter.setString("Name");
//     nameEnter.setPosition(Vector2f (30, 150));
//     nameEnter.setCharacterSize(20);

//     Text passwordEnter;

//     passwordEnter.setFont(defaultFont);
//     passwordEnter.setFillColor(Color::Black);
//     passwordEnter.setString("Password");
//     passwordEnter.setPosition(Vector2f (30, 240));
//     passwordEnter.setCharacterSize(20);

//     bool isPasswordInput = false;
//     bool isNameInput = false;

//     bool isInputError = false;

//     string playerNameInput;
//     string playerPasswordInput;

//     while (window.isOpen()){

//         Event event;
//         while (window.pollEvent(event)){
//             if (event.type == Event::Closed){
//                 window.close();
//                 return 1;
//             }
//         }

//         window.clear(swampGreenColor);

//         if (event.type == Event::TextEntered){   
//             if (isNameInput){            
//                 if (event.text.unicode == 8 && playerNameInput.length() > 0){
//                     playerNameInput.erase(playerNameInput.end() - 1);
//                     nameInputText.setString(playerNameInput);
//                 }
//                 else if (event.text.unicode < 128 && event.text.unicode != 8)
//                 {
//                     playerNameInput += event.text.unicode;
//                     nameInputText.setString(playerNameInput);
//                 }
//             }
//             else if (isPasswordInput){
//                 if (event.text.unicode == 8 && playerPasswordInput.length() > 0){
//                     playerPasswordInput.erase(playerPasswordInput.end() - 1);
//                     passwordInputText.setString(playerPasswordInput);
//                 }
//                 else if (event.text.unicode < 128 && event.text.unicode != 8){
//                     playerPasswordInput += event.text.unicode;
//                     passwordInputText.setString(playerPasswordInput);
//                 }
//             }
//         }

//         if (Mouse::isButtonPressed(Mouse::Left)){         
//             Vector2i mousePos = Mouse::getPosition(window);
//             isNameInput = false;
//             isPasswordInput = false;
//             if (nameInputBox.getGlobalBounds().contains(mousePos.x, mousePos.y)){
//                 isNameInput = true; 
//             }
//             else if (passwordInputBox.getGlobalBounds().contains(mousePos.x, mousePos.y)){
//                 isPasswordInput = true;
//             }

//             if (sentNameButton.getGlobalBounds().contains(mousePos.x, mousePos.y)){
//                 if (playerNameInput.length() > 0 && playerPasswordInput.length() > 0){
//                     if (loginUser(playerNameInput, playerPasswordInput)){
//                         window.close();
//                     }
//                     isInputError = false;
//                 }
//                 else {
//                     isInputError = true;
//                 }
                
//                 playerNameInput = "";
//                 playerPasswordInput = "";
//                 passwordInputText.setString(playerPasswordInput);
//                 nameInputText.setString(playerNameInput);
//             }
//         }

//         window.draw(nameInputBox);
//         window.draw(passwordInputBox);

//         window.draw(passwordEnter);
//         window.draw(nameEnter);

//         window.draw(passwordInputText);
//         window.draw(nameInputText);

//         window.draw(sentNameButton);
//         window.draw(sentNameText);

//         if (isInputError){
//             window.draw(inputError);
//         }

//         window.display(); 
//     }
//     return 0;
// }


void showShop (){

    RenderWindow shop(VideoMode(WIDTH, HIGHT), "shop");

    shop.setFramerateLimit(FRAME);

    RectangleShape operatesButton;

    operatesButton.setSize(Vector2f (30, 30)); 
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
    
    vector <Good> goods = createGoods(); 

    Text textTemplate;

    textTemplate.setFont(defaultFont);
    textTemplate.setFillColor(Color::Black);
    textTemplate.setCharacterSize(18);

    Text loginText;
    RectangleShape loginButton;

    loginText = textTemplate;
    loginText.setString("login");
    loginText.setPosition(520, 105);

    loginButton = operatesButton;

    loginButton.setSize(Vector2f (90, 30));
    loginButton.setPosition(500, 100);


    vector <Text> products;
    vector <Text> countProd;
    vector <Text> slesh;
    vector <Text> stockText;
    vector <string> stock;

    for (auto i = 0; i < goods.size(); i++){
        products.push_back(textTemplate);
        products[i].setString(goods[i].name);
        products[i].setPosition(Vector2f (x, y));

        countProd.push_back(textTemplate);
        countProd[i].setString(to_string(goods[i].count));
        countProd[i].setPosition(Vector2f (x + 350, y));

        stockText.push_back(textTemplate);
        stockText[i].setString(to_string(goods[i].count));
        stockText[i].setPosition(Vector2f (x + 320, y));

        stock.push_back(to_string(goods[i].count));

        slesh.push_back(textTemplate);
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
    
    vector <Order> orders;

    vector <Text> orderGoodName;
    vector <Text> orderGoodCount;

    bool succesLogin = false;

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
                    orders = editOrder(orders, button, goods);

                    stock[button] = to_string(editOne(stoi(stock[button]), false, goods[button].count));
                    stockText[button].setString(stock[button]);

                    orderGoodName = getDataGood(orders);
                    orderGoodCount = getDataCount(orders);

                    minButton = setDataButton(orders);
                    minusText = setDataText(orders);
                }
            }
            for (auto button = 0; button < orders.size(); button++){
                if (minButton[button].getGlobalBounds().contains(MousePos.x, MousePos.y)){ 
                    int index = stockName(orders[button].goodName, goods);
                    orders = editOrder(orders, button, goods, true);

                    stock[index] = to_string(editOne(stoi(stock[index]), true, goods[index].count));
                    stockText[index].setString(stock[index]);

                    orderGoodName = getDataGood(orders);
                    orderGoodCount = getDataCount(orders);

                    minButton = setDataButton(orders);
                    minusText = setDataText(orders);                    
                }
            }
            // if (loginButton.getGlobalBounds().contains(MousePos.x, MousePos.y)){
            //     succesLogin = checkLogin();
            // }
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

        shop.draw(loginButton);
        shop.draw(loginText);

        shop.display();
    }
}


int stockName(string str, vector <Good> goods){
    for (int i = 0; i < goods.size(); i++){
        if (str == goods[i].name){
            return i;
        }
    }
    return 0;
}


vector <Text> setDataText(vector <Order>  order){
    vector <Text> output;
    Text templateText;

    templateText.setFont(defaultFont);
    templateText.setFillColor(Color::Black);
    templateText.setCharacterSize(18);
    templateText.setString("-");

    int yMin = 400;
    int xMin = 190;

    for (int i = 0; i < order.size(); i++){
        output.push_back(templateText);
        output[i].setPosition(xMin + 5, yMin);
        yMin += 40;
    } 

    return output;   
}


vector <RectangleShape> setDataButton(vector <Order> orders){
    vector <RectangleShape> buttons;
   
    RectangleShape opButton;

    opButton.setSize(Vector2f(30, 30));
    opButton.setOutlineThickness(3);
    opButton.setOutlineColor(Color::Black);
    opButton.setFillColor(Color::White);  

    int yMin = 400;
    int xMin = 190;

    for(int i = 0; i < orders.size(); i++){
        buttons.push_back(opButton);
        buttons[i].setPosition(xMin, yMin);
        yMin += 40;
    }
    
    return buttons;
}


vector <Text> getDataGood(vector <Order> order){
    Text templateText;

    templateText.setFont(defaultFont);
    templateText.setFillColor(Color::Black);
    templateText.setCharacterSize(18);

    vector <Text> output;

    int x = 20; 
    int y = 400;

    for (int i = 0; i < order.size(); i++){
        output.push_back(templateText);
        output[i].setString(order[i].goodName);
        output[i].setPosition(x, y);
        y += 40;
    }

    return output;
}


vector <Text> getDataCount(vector <Order> orders){
    Text templateText;

    templateText.setFont(defaultFont);
    templateText.setFillColor(Color::Black);
    templateText.setCharacterSize(18);

    vector <Text> output;

    int x = 160; 
    int y = 400;

    for (int i = 0; i < orders.size(); i++){
        output.push_back(templateText);
        output[i].setString(to_string(orders[i].count));
        output[i].setPosition(x, y);
        y += 40;
    }

    return output;
}


// string deleteSpaceInString (string name){
//     string output = "";
//     for (int i = 0; i < name.length(); i++){
//         if (!isspace(name[i])){
//             output += name[i];
//         }
//     }
//     return output;
// }


// bool loginUser(string inputName, string inputPassword){
//     string name = deleteSpaceInString(inputName);
//     string password = deleteSpaceInString(inputPassword);

//     sqlite3 *db;
//     char *errMsg = nullptr;
//     string sql = "INSERT INTO USERS(username, password) VALUES('"+ name +"', '"+ password +"');";

//     int rc = sqlite3_open(WAYTODB, &db);

//     User user;
//     user.name = name;
//     user.password = password;

//     rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
    
//     if (rc != SQLITE_OK ){
//         sqlite3_exec(db, "SELECT * FROM USERS", loginCallback, &user, NULL);
//         fprintf(stderr, "SQL error: %s\n", errMsg);
//         sqlite3_free(errMsg);
//     } 
//     else {
//         user.successfullyCheck = true;
//         fprintf(stdout, "Records created successfully\n");
//     }

//     sqlite3_close(db);

//     if (user.successfullyCheck){
//         return true;
//     }
//     else {
//         return false;
//     }

// }


// static int loginCallback(void *unused, int count, char **data, char **columns){                   

//     User *user = static_cast<User*>(unused);
    
//     if (user->name == data[0] && user->password == data[1]){
//         user -> successfullyCheck = true;
//     }
    
//     return 0;
// }


vector <Good> createGoods(){

    sqlite3 *db;
    vector <Good> goods;

    sqlite3_open(WAYTODB, &db);
    sqlite3_exec(db, "SELECT * FROM GOODS", listOfGoodsCallback, &goods, NULL);
    sqlite3_close(db);

    return goods;
}


vector<Order> editOrder(vector <Order> orders, int button, vector <Good> goods){
    for (int i = 0; i < orders.size(); i++){
        if (orders[i].goodName == goods[button].name){
            orders[i].count = editOne(orders[i].count, true, goods[button].count);
            return orders;
        }
    }

    Order order;

    order.goodName = goods[button].name;
    order.count = 1;
    
    orders.push_back(order);

    return orders;
}


vector <Order> editOrder(vector <Order> orders, int button, vector <Good> goods, bool minus){
    for (int i = 0; i < orders.size(); i++){
        if (orders[i].goodName == orders[button].goodName){
            orders[i].count--;
            if (orders[i].count == 0){
                orders.erase(orders.begin() + i);
            }
        }
    }
    return orders;
}



int editOne(int num, bool add, int limit){;
    add ? num++ : num--;
    num = num > limit ? --num : num;
    return num < 0 ? 0 : num;
}


// void editCount(int button, Goods goods){

//     sqlite3 *db;
//     string sql;
//     sqlite3_open(WAYTODB, &db);
//     int miidleButton = button / 2;
//     if (button % 2 == 0){ 
//         sql = "UPDATE GOODS SET count = count + 1 WHERE name = '"+ goods.name[miidleButton] + "';";
//     }
//     else {
//         if (goods.count[miidleButton] != '0'){
//             sql = "UPDATE GOODS SET count = count - 1 WHERE name = '"+ goods.name[miidleButton] + "';";
//         }
//     }
//     sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
//     sqlite3_close(db);
// }


// vector <Good> updateGoods(){
//     vector <Good> goods;
//     sqlite3 *db;
//     sqlite3_open(WAYTODB, &db);

//     sqlite3_exec(db, "SELECT * FROM GOODS", listOfGoodsCallback, &goods, NULL);
//     sqlite3_close(db);

//     return goods;
// }


static int listOfGoodsCallback(void *unused, int count, char **data, char **columns){      

    vector <Good> *goods = static_cast<vector <Good>* >(unused);

    Good good;

    good.name = data[0];
    good.count = stoi(data[1]);
    
    goods -> push_back(good);
    
    return 0;
}


Font getFont(){ 

    Font font;
    font.loadFromFile("src/Mooli-Regular.ttf");

    return font;
}