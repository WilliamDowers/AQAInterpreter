#include <string>
#include <iostream>
#include <memory>
#include <vector>

class Statement;
class Expression;
class Visitor;
class Interpreter;
class Function;

#include "token.h"
#include "nodes.h"
#include "interpreter.h"
#include "callable.h"


Expression::Expression() {};




BooleanLiteral::BooleanLiteral(bool _val) { val = _val; }
Expression* BooleanLiteral::accept(Visitor* interpreter) { return interpreter->VisitBooleanLiteralExpression(this); }



StringLiteral::StringLiteral(std::string _val) { val = _val; }
Expression* StringLiteral::accept(Visitor* interpreter) { return interpreter->VisitStringLiteralExpression(this); }



NumericLiteral::NumericLiteral(Token _num) { val = (std::stod(_num.GetLexeme())); }
NumericLiteral::NumericLiteral(double _val) { val = _val; }
Expression* NumericLiteral::accept(Visitor* interpreter) { return interpreter->VisitNumericLiteralExpression(this); }



GroupExpression::GroupExpression(Expression* _expr) { expr = _expr; }
GroupExpression::~GroupExpression() { delete expr; }
Expression* GroupExpression::accept(Visitor* interpreter) { return interpreter->VisitGroupExpression(this); }



UnaryExpression::UnaryExpression(Token _operation, Expression* _expr) {
    operation = _operation;
    expr = _expr;
}
UnaryExpression::~UnaryExpression() {
    delete expr;
}

Expression* UnaryExpression::accept(Visitor* interpreter) { return interpreter->VisitUnaryExpression(this); }

CallExpression::CallExpression(Expression* _callee, std::vector<Expression*> arguments) {
    callee = _callee;
    args = arguments;
}

Expression* CallExpression::accept(Visitor* interpreter) { return interpreter->VisitCallExpression(this); }


BinaryExpression::BinaryExpression(Expression* _left, Token _operation, Expression* _right) {
    left = _left;
    operation = _operation;
    right = _right;
}
BinaryExpression::~BinaryExpression() {
    delete left;
    delete right;
}

Expression* BinaryExpression::accept(Visitor* interpreter) { return interpreter->VisitBinaryExpression(this); }


Statement::Statement() {};

PrintStatement::PrintStatement(Expression* expression) { expr = expression; }

Statement* PrintStatement::accept(Visitor* interpreter) { 
    interpreter->VisitPrintStatement(this); 
    return nullptr;
}

ExpressionStatement::ExpressionStatement(Expression* expression) { expr = expression; }

Statement* ExpressionStatement::accept(Visitor* interpreter) {
    interpreter->VisitExpressionStatement(this);
    return nullptr;
}


VariableDeclarationStatement::VariableDeclarationStatement(Token identifier, Expression* value) { 
    name = identifier;
    val = value;
}

Statement* VariableDeclarationStatement::accept(Visitor* interpreter) { 
    interpreter->VisitVariableDeclarationStatement(this); 
    return nullptr;
}

FunctionDeclarationStatement::FunctionDeclarationStatement(Token name, std::vector<Token> params, std::vector<Statement*> bdy) {
    identifier = name;
    parameters = params;
    body = bdy;
}

Statement* FunctionDeclarationStatement::accept(Visitor* interpreter) { 
    interpreter->VisitFunctionDeclarationStatement(this);
    return nullptr;
}

ReturnStatement::ReturnStatement(Expression* value) { returnValue = value;}

Statement* ReturnStatement::accept(Visitor* interpreter) {
    interpreter->VisitReturnStatement(this);
    return nullptr;
}

VariableExpression::VariableExpression(Token identifier) { name = identifier; }
Expression* VariableExpression::accept(Visitor* interpreter) { return interpreter->VisitVariableExpression(this); }


VariableAssignmentExpression::VariableAssignmentExpression(Token identifier, Expression* value) {
    name = identifier;
    val = value;
}

Expression* VariableAssignmentExpression::accept(Visitor* interpreter) { return interpreter->VisitVariableAssignmentExpression(this); }


BlockStatement::BlockStatement(std::vector<Statement*> stmts) { statements = stmts; }

Statement* BlockStatement::accept(Visitor* interpreter) { 
    interpreter->VisitBlockStatement(this);
    return nullptr;
}


IfStatement::IfStatement(Expression* cond, Statement* brnch, Statement* elseBrnch)  {
    condition = cond;
    branch = brnch;
    elseBranch = elseBrnch;
}

Statement* IfStatement::accept(Visitor* interpreter) { 
    interpreter->VisitIfStatement(this);
    return nullptr;
}


WhileStatement::WhileStatement(Expression* cond, Statement* bdy) {
    condition = cond;
    body = bdy;
}

Statement* WhileStatement::accept(Visitor* interpreter) {
    interpreter->VisitWhileStatement(this);
    return nullptr;
}

ForStatement::ForStatement(Token it, Expression* begin, Expression* finish, Statement* bdy) {
    iterator = it;
    start = begin;
    end = finish;
    body = bdy;
}

Statement* ForStatement::accept(Visitor* interpreter) {
    interpreter->VisitForStatement(this);
    return nullptr;
}


InputExpression::InputExpression() {

}

Expression* InputExpression::accept(Visitor* interpreter) {
	return interpreter->VisitInputExpression(this);
}