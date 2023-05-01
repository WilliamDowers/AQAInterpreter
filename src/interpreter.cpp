#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

class Statement;
class Expression;
class Visitor;
class Interpreter;
class Printer;
class Environment;
class Value;
class Function;

#include "token.h"
#include "nodes.h"
#include "visitor.h"
#include "interpreter.h"
#include "ASTprinter.h"
#include "environment.h"
#include "callable.h"



ReturnException::ReturnException(Expression* expr) { value = expr; }

Interpreter::Interpreter() {
    globals = new Environment();
    environment = globals;
}

void Interpreter::Interpret(std::vector<Statement*> statements) {
    environment = new Environment();
    for (Statement* stmt : statements) {
        execute(stmt);
    }
}


NumericLiteral* Interpreter::VisitNumericLiteralExpression(NumericLiteral* numericLiteral) { return numericLiteral; }
StringLiteral* Interpreter::VisitStringLiteralExpression(StringLiteral* stringLiteral) { return stringLiteral; }
BooleanLiteral* Interpreter::VisitBooleanLiteralExpression(BooleanLiteral* booleanLiteral) { return booleanLiteral; }

Expression* Interpreter::VisitGroupExpression(GroupExpression* groupExpression) { return groupExpression->expr; }

Expression* Interpreter::VisitUnaryExpression(UnaryExpression* unaryExpression) { 
    Expression* right = evaluate(unaryExpression->expr);
    if(unaryExpression->operation.GetType() == TOKEN_TYPE::MINUS) {
        return new NumericLiteral(-1*((NumericLiteral*)right)->val);
    }
    return new NumericLiteral(((NumericLiteral*)right)->val);
}

 // I dont have a fucking clue what this function actually does anymore but it works so leave it alone 
Expression* Interpreter::VisitCallExpression(CallExpression* callExpression) {
    Expression* callee = evaluate(callExpression->callee); // DO NOT DEREFERENCE USES DIRTY HACK TO WORK AROUND MY POORLY DESIGNED SYSTEM

    std::vector<Expression*> arguments;
    for (Expression* arg : callExpression->args) {
        arguments.push_back(evaluate(arg));
    }

    if (reinterpret_cast<Function*>(callee) == nullptr) {
        throw std::invalid_argument("Can only call subroutines");
    }



    Function* subroutine = reinterpret_cast<Function*>(callee);

    if (arguments.size() != subroutine->Arity()) {
        throw std::runtime_error("Expected " + std::to_string(subroutine->Arity()) + " arguments but " + std::to_string(arguments.size()) + " were given");
    }

    return subroutine->call(this, arguments);
}





Expression* Interpreter::VisitBinaryExpression(BinaryExpression* binaryExpression) { 
    Expression* left = evaluate(binaryExpression->left);
    Expression* right = evaluate(binaryExpression->right);
    switch(binaryExpression->operation.GetType()) {
        case TOKEN_TYPE::PLUS:
            if (dynamic_cast<NumericLiteral*>(left) != nullptr && dynamic_cast<NumericLiteral*>(right) != nullptr) {
                return new NumericLiteral(((NumericLiteral*)left)->val + ((NumericLiteral*)right)->val);
            }
            if (dynamic_cast<StringLiteral*>(left) != nullptr && dynamic_cast<NumericLiteral*>(right) != nullptr) {
                double rght = ((NumericLiteral*)right)->val;
                if (std::floor(rght) == rght) { return new StringLiteral(((StringLiteral*)left)->val.append(std::to_string(int(rght)))); }
                else { return new StringLiteral(((StringLiteral*)left)->val + (std::to_string(rght))); }
            }
            if (dynamic_cast<StringLiteral*>(left) != nullptr && dynamic_cast<StringLiteral*>(right) != nullptr) {
                return new StringLiteral(((StringLiteral*)left)->val + (((StringLiteral*)right)->val));
            }
            throw std::invalid_argument("Operands of '+' must evaluate to: (NumericLiteral, NumericLiteral), (StringLiteral, StringLiteral), (StringLiteral, NumericLiteral)");
        case TOKEN_TYPE::MINUS:
            if (dynamic_cast<NumericLiteral*>(left) == nullptr || dynamic_cast<NumericLiteral*>(right) == nullptr) { throw std::invalid_argument("Operands of '-' must evaluate to type NumericLiteral"); }
            return new NumericLiteral(((NumericLiteral*)left)->val - ((NumericLiteral*)right)->val);
        case TOKEN_TYPE::STAR:
            if (dynamic_cast<NumericLiteral*>(left) == nullptr || dynamic_cast<NumericLiteral*>(right) == nullptr) { throw std::invalid_argument("Operands of '*' must evaluate to type NumericLiteral"); }
            return new NumericLiteral(((NumericLiteral*)left)->val * ((NumericLiteral*)right)->val);
        case TOKEN_TYPE::SLASH:
            if (dynamic_cast<NumericLiteral*>(left) == nullptr || dynamic_cast<NumericLiteral*>(right) == nullptr) { throw std::invalid_argument("Operands of '/' must evaluate to type NumericLiteral"); }
            return new NumericLiteral(((NumericLiteral*)left)->val /  ((NumericLiteral*)right)->val);
        case TOKEN_TYPE::GREATER_THAN:
            if (dynamic_cast<NumericLiteral*>(left) == nullptr || dynamic_cast<NumericLiteral*>(right) == nullptr) { throw std::invalid_argument("Operands of '>' must evaluate to type NumericLiteral"); }
            return new BooleanLiteral(((NumericLiteral*)left)->val > ((NumericLiteral*)right)->val);
        case TOKEN_TYPE::LESS_THAN:
            if (dynamic_cast<NumericLiteral*>(left) == nullptr || dynamic_cast<NumericLiteral*>(right) == nullptr) { throw std::invalid_argument("Operands of '<' must evaluate to type NumericLiteral"); }
            return new BooleanLiteral(((NumericLiteral*)left)->val < ((NumericLiteral*)right)->val);
        case TOKEN_TYPE::GREATER_EQ_THAN:
            if (dynamic_cast<NumericLiteral*>(left) == nullptr || dynamic_cast<NumericLiteral*>(right) == nullptr) { throw std::invalid_argument("Operands of '>=' must evaluate to type NumericLiteral"); }
            return new BooleanLiteral(((NumericLiteral*)left)->val >= ((NumericLiteral*)right)->val);
        case TOKEN_TYPE::LESS_EQ_THAN:
            if (dynamic_cast<NumericLiteral*>(left) == nullptr || dynamic_cast<NumericLiteral*>(right) == nullptr) { throw std::invalid_argument("Operands of '<=' must evaluate to type NumericLiteral"); }
            return new BooleanLiteral(((NumericLiteral*)left)->val <= ((NumericLiteral*)right)->val);
        case TOKEN_TYPE::NOT_EQUAL:
            if (dynamic_cast<NumericLiteral*>(left) == nullptr || dynamic_cast<NumericLiteral*>(right) == nullptr) { throw std::invalid_argument("Operands of '!=' must evaluate to type NumericLiteral"); }
            return new BooleanLiteral(((NumericLiteral*)left)->val != ((NumericLiteral*)right)->val);
        case TOKEN_TYPE::EQUAL:
            if (dynamic_cast<NumericLiteral*>(left) == nullptr || dynamic_cast<NumericLiteral*>(right) == nullptr) { throw std::invalid_argument("Operands of '=' must evaluate to type NumericLiteral"); }
            return new BooleanLiteral(((NumericLiteral*)left)->val == ((NumericLiteral*)right)->val);
        default:
            return nullptr;
    }
}

void Interpreter::VisitBlockStatement(BlockStatement* blockStatement) {
    executeBlock(blockStatement->statements, new Environment(environment));
}

Expression* Interpreter::VisitVariableExpression(VariableExpression* variableExpression) {
    Value val = environment->Get(variableExpression->name.GetLexeme());
    switch (val.valueType) {
        case VALUE_TYPE::BOOLEAN:
            return new BooleanLiteral(val.literal.boolean);
        case VALUE_TYPE::STRING:
            return new StringLiteral(*(val.literal.string));
        case VALUE_TYPE::NUMBER:
            return new NumericLiteral(val.literal.number);
        case VALUE_TYPE::FUNCTION:
            return reinterpret_cast<Expression*>(val.literal.function); // dirty hack DO NOT DEREFERENCE 
    }
} 

Expression* Interpreter::VisitVariableAssignmentExpression(VariableAssignmentExpression* variableAssignmentExpression) {
    Expression* value = evaluate(variableAssignmentExpression->val);
    if (dynamic_cast<NumericLiteral*>(value) != nullptr) {
        environment->Assign(variableAssignmentExpression->name.GetLexeme(), dynamic_cast<NumericLiteral*>(value)->val);
    }
    else if (dynamic_cast<StringLiteral*>(value) != nullptr) {
        environment->Assign(variableAssignmentExpression->name.GetLexeme(), dynamic_cast<StringLiteral*>(value)->val);
    }
    else if (dynamic_cast<BooleanLiteral*>(value) != nullptr) {
        environment->Assign(variableAssignmentExpression->name.GetLexeme(), dynamic_cast<BooleanLiteral*>(value)->val);
    }
    else {
        //report a runtime error
    }
    return value;
}


void Interpreter::VisitExpressionStatement(ExpressionStatement* expressionStatement) {
    Expression* expr = evaluate(expressionStatement->expr);
}
void Interpreter::VisitPrintStatement(PrintStatement* printStatement) {
    Expression* expr = evaluate(printStatement->expr);
    printer.Print(expr);
}

void Interpreter::VisitVariableDeclarationStatement(VariableDeclarationStatement* variableStatement) {
    Expression* value = evaluate(variableStatement->val);
    if (dynamic_cast<NumericLiteral*>(value) != nullptr) {
        environment->Declare(variableStatement->name.GetLexeme(), dynamic_cast<NumericLiteral*>(value)->val);
    }
    else if (dynamic_cast<StringLiteral*>(value) != nullptr) {
        environment->Declare(variableStatement->name.GetLexeme(), dynamic_cast<StringLiteral*>(value)->val);
    }
    else if (dynamic_cast<BooleanLiteral*>(value) != nullptr) {
        environment->Declare(variableStatement->name.GetLexeme(), dynamic_cast<BooleanLiteral*>(value)->val);
    }
    else {

    }
}


void Interpreter::VisitFunctionDeclarationStatement(FunctionDeclarationStatement* functionDeclarationStatement) {
    Function* function = new Function(functionDeclarationStatement);
    environment->Declare(functionDeclarationStatement->identifier.GetLexeme(), function);
}


void Interpreter::VisitReturnStatement(ReturnStatement* returnStatement) {
    Expression* value = nullptr;
    if (returnStatement->returnValue != nullptr) {
        value = evaluate(returnStatement->returnValue);
    }
    throw ReturnException(value);
}

void Interpreter::VisitIfStatement(IfStatement* ifStatement) {
    if (dynamic_cast<BooleanLiteral*>(evaluate(ifStatement->condition))->val) {
        execute(ifStatement->branch);
    }
    else if (dynamic_cast<BooleanLiteral*>(evaluate(ifStatement->condition))->val == false && ifStatement->elseBranch != nullptr) {
        execute(ifStatement->elseBranch);
    }
}

void Interpreter::VisitWhileStatement(WhileStatement* whileStatement) {
    while (dynamic_cast<BooleanLiteral*>(evaluate(whileStatement->condition))->val) {
        execute(whileStatement->body);
    }
}

void Interpreter::VisitForStatement(ForStatement* forStatement) {
    Environment* previous = environment;
    environment = new Environment(environment);
    environment->Declare(forStatement->iterator.GetLexeme(), dynamic_cast<NumericLiteral*>(evaluate(forStatement->start))->val);
    while(environment->Get(forStatement->iterator.GetLexeme()).literal.number < dynamic_cast<NumericLiteral*>((forStatement->end))->val) {
        execute(forStatement->body);
        environment->Assign(forStatement->iterator.GetLexeme(), environment->Get(forStatement->iterator.GetLexeme()).literal.number+1);
    }
    delete environment;
    environment = previous;
}

Expression* Interpreter::evaluate(Expression* expr) { 
    return expr->accept(this);
}

void Interpreter::execute(Statement* stmt) {
    stmt->accept(this);
}

void Interpreter::executeBlock(std::vector<Statement*> stmts, Environment* env) {
    Environment* current = environment;
    environment = env;
    for (Statement* stmt : stmts) {
        execute(stmt);
    }
    environment = current;
}