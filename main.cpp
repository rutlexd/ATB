#include <iostream>
#include <cstring>
#include <vector>
#include "src/sqlite3.h"
#include "class.h"
#include "sfml.h"

using namespace std;
using namespace sf;

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
vector <Order> editOrder(vector <Order> orders, int button, vector <Good> goods);
vector <Order> editOrder(vector <Order> orders, int button, vector <Good> goods, bool minus);
int editOne(int num, bool add, int limit);
vector <Text> updateOrderGoodsCount(vector <Order> orders);
vector <Text> updateOrderGoods(vector <Order> orders);
vector <RectangleShape> createButton(vector <Order> orders);
vector <Text> createText(vector <Order> orders);
int getNameIndex (string str, vector <Good> goods);

const char *DB_WAY = "src/Database.db";

int main(){

    showShop();

    return 0;
}


bool checkLogin(){
    
    const int loginWindowWidth = 300;
    const int loginWindowHight = 600;

    const int BoxWidth = 120;
    const int BoxHight = 30;
    const int middleBox = BoxWidth / 4;
    const int BoxPosX = 140;
    const int nameInputBoxPosY = 150;
    const int passwordInputBoxPosY = 240;
    const int sentNamePosY = 340;
    const int textPosX = 30;
    const int inputErrorPosY = 200;
    const int nameEnterPosY = 150;
    const int passwordEnterPosY = 240;

    RenderWindow window(VideoMode(loginWindowWidth, loginWindowHight), "login");
    
    window.setFramerateLimit(FRAME);

    Text templateText;

    templateText.setFont(DEFAULT_FONT);
    templateText.setFillColor(Color::Black);
    templateText.setCharacterSize(14);

    RectangleShape templateButton;

    templateButton.setFillColor(Color::White);
    templateButton.setOutlineThickness(5);
    templateButton.setOutlineColor(Color::Black);
    templateButton.setSize(Vector2f(BoxWidth, BoxHight));

    RectangleShape nameInputBox;

    nameInputBox = templateButton; 
    nameInputBox.setPosition(Vector2f (BoxPosX, nameInputBoxPosY));

    RectangleShape passwordInputBox;

    passwordInputBox = templateButton;
    passwordInputBox.setPosition(Vector2f (BoxPosX, passwordInputBoxPosY));

    RectangleShape sentNameButton;

    sentNameButton = templateButton;
    sentNameButton.setFillColor(coffeeColor);
    sentNameButton.setPosition(Vector2f (BoxPosX, sentNamePosY));

    Text nameInputText;

    nameInputText = templateText;
    nameInputText.setPosition(Vector2f (BoxPosX, nameInputBoxPosY));
    

    Text passwordInputText;

    passwordInputText = templateText;
    passwordInputText.setPosition(Vector2f(BoxPosX, passwordInputBoxPosY));

    Text sentNameText;

    sentNameText = templateText;
    sentNameText.setString("Enter");
    sentNameText.setPosition(Vector2f (BoxPosX + middleBox, sentNamePosY));

    Text inputError;

    inputError = templateText;
    inputError.setFillColor(Color::Red);
    inputError.setString("Error, try again");
    inputError.setPosition(Vector2f (textPosX, inputErrorPosY));
    inputError.setCharacterSize(16);

    Text nameEnter;

    nameEnter = templateText;
    nameEnter.setString("Name");
    nameEnter.setPosition(Vector2f (textPosX, nameEnterPosY));
    nameEnter.setCharacterSize(20);

    Text passwordEnter;

    passwordEnter = templateText;
    passwordEnter.setString("Password");
    passwordEnter.setPosition(Vector2f (textPosX, passwordEnterPosY));
    passwordEnter.setCharacterSize(20);

    bool isPasswordInput = false;
    bool isNameInput = false;
    bool isInputError = false;

    string playerNameInput;
    string playerPasswordInput;

    bool unpressCheck;

    while (window.isOpen()){

        Event event;
        while (window.pollEvent(event)){
            if (event.type == Event::Closed){
                window.close();
                return false;
            }
        }

        window.clear(swampGreenColor);
        
        if (event.type != Event::TextEntered){
            unpressCheck = true;
        }

        if (event.type == Event::TextEntered){
            int textCode = event.text.unicode;

            while(event.type == Event::TextEntered && unpressCheck == true) {
                if (isNameInput){            
                    if (textCode == 8 && playerNameInput.length() > 0){
                        playerNameInput.erase(playerNameInput.end() - 1);
                        nameInputText.setString(playerNameInput);
                    }
                    else if (textCode < 128 && textCode != 8)
                    {
                        playerNameInput += textCode;
                        nameInputText.setString(playerNameInput);
                    }
                }
                else if (isPasswordInput){
                    if (textCode == 8 && playerPasswordInput.length() > 0){
                        playerPasswordInput.erase(playerPasswordInput.end() - 1);
                        passwordInputText.setString(playerPasswordInput);
                    }
                    else if (textCode < 128 && textCode != 8){
                        playerPasswordInput += textCode;
                        passwordInputText.setString(playerPasswordInput);
                    }
                }
                unpressCheck = false;
            } ;
        
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
                        return true;
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
    return false;
}


void showShop (){

    RenderWindow shop(VideoMode(WIDTH, HIGHT), "shop");

    shop.setFramerateLimit(FRAME);

    RectangleShape operatesButton;
    int opButtonWidth = 30;
    int opButtonHight = 30;

    operatesButton.setSize(Vector2f (opButtonWidth, opButtonHight)); 
    operatesButton.setFillColor(Color::White);
    operatesButton.setOutlineThickness(5);
    operatesButton.setOutlineColor(Color::Black);

    Text plus;
    char plusSymbol = '+';

    plus.setFont(DEFAULT_FONT);
    plus.setFillColor(Color::Black);
    plus.setCharacterSize(24);
    plus.setString(plusSymbol);

    Text orderText = plus;
    int orderTextX = 200;
    int orderTextY = 350;

    orderText.setPosition(orderTextX, orderTextY);
    orderText.setString("Order:");    

    vector <Good> goods = createGoods(); 

    Text textTemplate;

    textTemplate.setFont(DEFAULT_FONT);
    textTemplate.setFillColor(Color::Black);
    textTemplate.setCharacterSize(18);

    RectangleShape loginButton;

    int loginButtonWidth = 90;
    int loginButtonHight = 30;
    int loginButtonPosX = 500;
    int loginButtonPosY = 100;

    loginButton = operatesButton;
    loginButton.setSize(Vector2f (loginButtonWidth, loginButtonHight));
    loginButton.setPosition(loginButtonPosX, loginButtonPosY);

    Text loginText;
    int loginTextPosX = loginButtonPosX + loginButtonWidth / 4;
    int loginTextPosY = loginButtonPosY + loginButtonHight / 4;

    loginText = textTemplate;
    loginText.setString("login");
    loginText.setPosition(loginTextPosX, loginTextPosY);
   
    vector <Text> products;
    vector <Text> countProd;
    vector <Text> separator;
    vector <Text> countSaleText;
    vector <int> countSale;

    const char separatorSymbol = '/';

    int globalPosY = 20;

    int productsPosX = 20;
    int countProdPosX = 370;
    int countSalePosX = 340;
    int separatorPosX = 360;

    for (auto i = 0; i < goods.size(); i++){
        products.push_back(textTemplate);
        products[i].setString(goods[i].name);
        products[i].setPosition(Vector2f (productsPosX, globalPosY));

        countProd.push_back(textTemplate);
        countProd[i].setString(to_string(goods[i].count));
        countProd[i].setPosition(Vector2f (countProdPosX, globalPosY));

        countSaleText.push_back(textTemplate);
        countSaleText[i].setString(to_string(goods[i].count));
        countSaleText[i].setPosition(Vector2f (countSalePosX, globalPosY));

        countSale.push_back(goods[i].count);

        separator.push_back(textTemplate);
        separator[i].setString(separatorSymbol);
        separator[i].setPosition(separatorPosX, globalPosY);

        globalPosY += ROW_HIGHT;
    }

    int plusPosY = 20;
    int plusPosX = 180;
    int plusTextPosX = plusPosX + opButtonWidth / 4;
    RectangleShape opButton;

    opButton.setSize(Vector2f(opButtonWidth,opButtonHight));
    opButton.setOutlineThickness(3);
    opButton.setOutlineColor(Color::Black);
    opButton.setFillColor(Color::White);
   
    vector <RectangleShape> plusButton;
    vector <RectangleShape> minusButton;
    vector <Text> plusButtonText;
    vector <Text> minusButtonText;

    for (auto i = 0; i < products.size(); i++){
        plusButton.push_back(opButton);
        plusButtonText.push_back(plus);
        plusButton[i].setPosition(plusPosX, plusPosY);
        plusButtonText[i].setPosition(plusTextPosX, plusPosY);
        plusPosY += ROW_HIGHT;
    }
    
    vector <Order> orders;
    vector <Text> orderGoodName;
    vector <Text> orderGoodCount;

    bool succesLogin = false;
    bool updateChecker = true;


    while (shop.isOpen()){
        Event event;
        while (shop.pollEvent(event)){
            if (event.type == Event::Closed){
                shop.close();
            }
        }

        

        if (Mouse::isButtonPressed(Mouse::Left)){
            Vector2i MousePos = Mouse::getPosition(shop);
            for (auto buttonIndex = 0; buttonIndex < products.size(); buttonIndex++){
                if (plusButton[buttonIndex].getGlobalBounds().contains(MousePos.x, MousePos.y)){ 
                    orders = editOrder(orders, buttonIndex, goods);

                    countSale[buttonIndex] = editOne(countSale[buttonIndex], false, goods[buttonIndex].count);
                    string saleText = to_string(countSale[buttonIndex]);
                    countSaleText[buttonIndex].setString(saleText);

                    orderGoodName = updateOrderGoods(orders);
                    orderGoodCount = updateOrderGoodsCount(orders);

                    minusButton = createButton(orders);
                    minusButtonText = createText(orders);

                    updateChecker = true;
                    break;
                }
            } // TODO  create optimization to end
            for (auto buttonIndex = 0; buttonIndex < orders.size(); buttonIndex++){
                cout << "w" << endl;
                bool isContainsMouse = minusButton[buttonIndex].getGlobalBounds().contains(MousePos.x, MousePos.y);
                if (isContainsMouse){ 
                    int index = getNameIndex(orders[buttonIndex].goodName, goods);
                    orders = editOrder(orders, buttonIndex, goods, true);

                    countSale[index] = editOne(countSale[index], true, goods[index].count);
                    string saleText = to_string(countSale[buttonIndex]);
                    countSaleText[index].setString(saleText);

                    orderGoodName = updateOrderGoods(orders);
                    orderGoodCount = updateOrderGoodsCount(orders);

                    minusButton = createButton(orders);
                    minusButtonText = createText(orders);
                    updateChecker = true;
                    break;
                }
            }
            if (loginButton.getGlobalBounds().contains(MousePos.x, MousePos.y)){
                cout << "e" << endl;
                succesLogin = checkLogin();
                if (succesLogin){
                    cout << "hip hip" << endl; // TODO logic for login
                }
            }
        }        

        if(updateChecker){
            shop.clear(Color::Blue);
        
            for (auto i = 0; i < products.size(); i++){
                shop.draw(products[i]);
                shop.draw(countProd[i]);

                shop.draw(separator[i]);
                shop.draw(countSaleText[i]);

                shop.draw(plusButton[i]);
                shop.draw(plusButtonText[i]);
            }

            for (auto i = 0; i < orderGoodName.size(); i++){
                shop.draw(orderGoodName[i]);
                shop.draw(orderGoodCount[i]);

                shop.draw(minusButton[i]);
                shop.draw(minusButtonText[i]);                        
            }

            shop.draw(orderText);
            shop.draw(loginButton);
            shop.draw(loginText);

            updateChecker = false;
        }

        shop.display();
    }
}


int getNameIndex(string name, vector <Good> goods){

    for (int i = 0; i < goods.size(); i++){
        if (name == goods[i].name){
            return i;
        }
    }

    return -1;
}


vector <Text> createText(vector <Order>  order){

    vector <Text> output;

    Text minusText;
    const char minusSymbol = '-';

    minusText.setFont(DEFAULT_FONT);
    minusText.setFillColor(Color::Black);
    minusText.setCharacterSize(18);
    minusText.setString(minusSymbol);

    int minusPosY = 400;
    int minusPosX = 200;

    for (int i = 0; i < order.size(); i++){
        output.push_back(minusText);
        output[i].setPosition(minusPosX, minusPosY);
        minusPosY += ROW_HIGHT;
    } 

    return output;   
}


vector <RectangleShape> createButton(vector <Order> orders){

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
        yMin += ROW_HIGHT;
    }
    
    return buttons;
}


vector <Text> updateOrderGoods(vector <Order> order){

    Text templateText;

    templateText.setFont(DEFAULT_FONT);
    templateText.setFillColor(Color::Black);
    templateText.setCharacterSize(18);

    vector <Text> output;

    int x = 20; 
    int y = 400;

    for (int i = 0; i < order.size(); i++){
        output.push_back(templateText);
        output[i].setString(order[i].goodName);
        output[i].setPosition(x, y);
        y += ROW_HIGHT;
    }

    return output;
}


vector <Text> updateOrderGoodsCount(vector <Order> orders){

    Text templateText;

    templateText.setFont(DEFAULT_FONT);
    templateText.setFillColor(Color::Black);
    templateText.setCharacterSize(18);

    vector <Text> output;

    int x = 160; 
    int y = 400;

    for (int i = 0; i < orders.size(); i++){
        output.push_back(templateText);
        output[i].setString(to_string(orders[i].count));
        output[i].setPosition(x, y);
        y += ROW_HIGHT;
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

    int rc = sqlite3_open(DB_WAY, &db);

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


vector <Good> createGoods(){

    sqlite3 *db;
    vector <Good> goods;

    sqlite3_open(DB_WAY, &db);
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


int editOne(int num, bool add, int limit){

    add ? num++ : num--;
    
    if (num > limit){
        num--;
    }
    else if (num < 0){
        num++;
    }

    return num;
}


static int listOfGoodsCallback(void *unused, int count, char **data, char **columns){      

    vector <Good> *goods = static_cast<vector <Good>* >(unused);

    Good good;

    good.name = data[0];
    good.count = stoi(data[1]);
    
    goods -> push_back(good);
    
    return 0;
}