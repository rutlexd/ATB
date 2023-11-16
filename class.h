#include <string>

struct User
{
    std::string name;
    std::string password;
    bool successfullyCheck = false;
};

struct Good
{
    std::string name;
    int count;
};

struct Order
{
    std::string goodName;
    int count;
    User user;
};