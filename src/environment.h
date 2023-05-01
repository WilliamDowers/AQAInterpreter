#pragma once



#include <unordered_map>
#include "token.h"

class Environment;

typedef std::unordered_map<std::string, class Value> map;

enum class VALUE_TYPE {
    BOOLEAN, NUMBER, STRING, FUNCTION
};

union value {
    double number;
    std::string* string;
    bool boolean;
    Function* function;
};

class Value {
public:
    Value(double val);
    Value(std::string val);
    Value(bool val);
    Value(Function* fun);
    Value() {};
    ~Value();

    VALUE_TYPE valueType;
    value   literal;
    
};

class Environment {
public:


    void Declare(std::string name, double val);
    void Declare(std::string name, bool val);
    void Declare(std::string name, std::string val);
    void Declare(std::string name, Function* fun);

    bool Assign(std::string name, double val);
    bool Assign(std::string name, bool val);
    bool Assign(std::string name, std::string val);
    bool Assign(std::string name, Function* fun);

    void SetParent(Environment* par);
    Environment();
    Environment(Environment* par);


    Value Get(std::string name);



private:
    Environment* parent;
    map variables;
};