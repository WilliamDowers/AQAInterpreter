#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

class Function;
class Expression;
class Interpreter;
class Visitor;

#include "token.h"
#include "nodes.h"
#include "environment.h"
#include "interpreter.h"
#include "visitor.h"
#include "callable.h"

Value::Value(double val) {
    valueType = VALUE_TYPE::NUMBER;
    literal.number = val;
}


Value::Value(std::string val) {
    valueType = VALUE_TYPE::STRING;
    literal.string = new std::string(val);
}

Value::Value(bool val) {
    valueType = VALUE_TYPE::BOOLEAN;
    literal.boolean = val;
}

Value::Value(Function* fun) {
    valueType = VALUE_TYPE::FUNCTION;
    literal.function = fun;
}


Value::~Value() {
    if (valueType == VALUE_TYPE::STRING) {
        delete literal.string;
    }
}

void Environment::SetParent(Environment* par) {
    parent = par;
}

Environment::Environment() {
    parent = nullptr;
}
Environment::Environment(Environment* par) {
    parent = par;
}

void Environment::Declare(std::string name, double val) {
    if (!Assign(name, val)) {
        variables[name] = val;
    }
}

void Environment::Declare(std::string name, bool val) {
    if (!Assign(name, val)) {
        variables[name] = val;
    }
}

void Environment::Declare(std::string name, std::string val) {
    if (!Assign(name, val)) {
        variables[name] = val;
    }
}

void Environment::Declare(std::string name, Function* fun) {
    if (!Assign(name, fun)) {
        variables[name] = fun;
    }
}


bool Environment::Assign(std::string name, double val) {
    if (variables.contains(name)) {
        variables[name] = Value(val);
        return true;
    }
    if (parent != nullptr) {
        parent->Assign(name, val);
    }
    
}

bool Environment::Assign(std::string name, bool val) {
    if (variables.contains(name)) {
        variables[name] = Value(val);
        return true;
    }
    if (parent != nullptr) {
        return parent->Assign(name, val);
    }
    
}

bool Environment::Assign(std::string name, std::string val) {
    if (variables.contains(name)) {
        variables[name] = Value(val);
        return true;
    }
    if (parent != nullptr) {
        return parent->Assign(name, val);
    }
    
}

bool Environment::Assign(std::string name, Function* fun) {
    if (variables.contains(name)) {
        variables[name] = Value(fun);
        return true;
    }
    if (parent != nullptr) {
        return parent->Assign(name, fun);
    }
}

Value Environment::Get(std::string name)  {
        if (variables.contains(name)) {
            return variables[name];
        }

        if (parent != nullptr) {
            return parent->Get(name);
        }
        throw std::runtime_error('\''+name+'\''+" does not exist");
    }