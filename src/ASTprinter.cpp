

#include <string>
#include <iostream>
#include <vector>

class Expression;
class Visitor;
class Printer;

#include "token.h"
#include "nodes.h"
#include "visitor.h"
#include "ASTprinter.h"

void Printer::Print(Expression* expression) {
    Expression* completed = print(expression);
    std::cout<<"\n";
    return;
}


NumericLiteral* Printer::VisitNumericLiteralExpression(NumericLiteral* numericLiteral) { std::cout<<numericLiteral->val; return nullptr; }
StringLiteral* Printer::VisitStringLiteralExpression(StringLiteral* stringLiteral) { std::cout<<stringLiteral->val; return nullptr; }
BooleanLiteral* Printer::VisitBooleanLiteralExpression(BooleanLiteral* booleanLiteral) { 
    if (booleanLiteral->val) { std::cout<< "TRUE"; }
    else { std::cout<< "FALSE"; }
    return nullptr;    
}

Expression* Printer::VisitGroupExpression(GroupExpression* groupExpression) { 
    std::cout<<'(';
    print(groupExpression->expr);
    std::cout<<')';
    return nullptr;
}
Expression* Printer::VisitUnaryExpression(UnaryExpression* unaryExpression) { 
    if(unaryExpression->operation.GetType() == TOKEN_TYPE::MINUS) {
        std::cout<<"-(";
        Expression* right = print(unaryExpression->expr);
        std::cout<<")";
        return nullptr;
    }
    print(unaryExpression->expr);
    return nullptr;
}

Expression* Printer::VisitCallExpression(CallExpression* callExpression) {

}


Expression* Printer::VisitBinaryExpression(BinaryExpression* binaryExpression) { 
    Expression* left = print(binaryExpression->left);
    switch(binaryExpression->operation.GetType()) {
        case TOKEN_TYPE::PLUS:
            std::cout<<'+'; break;
        case TOKEN_TYPE::MINUS:
            std::cout<<'-'; break;
        case TOKEN_TYPE::STAR:
            std::cout<<'*'; break;
        case TOKEN_TYPE::SLASH:
            std::cout<<'/'; break;
        case TOKEN_TYPE::GREATER_THAN:
            std::cout<<'>'; break;
        case TOKEN_TYPE::LESS_THAN:
            std::cout<<'<'; break;
        case TOKEN_TYPE::GREATER_EQ_THAN:
            std::cout<<">="; break;
        case TOKEN_TYPE::LESS_EQ_THAN:
            std::cout<<"<="; break;
        case TOKEN_TYPE::NOT_EQUAL:
            std::cout<<"!="; break;
        case TOKEN_TYPE::EQUAL:
            std::cout<<'='; break;
        default:
            break;
    }
    Expression* right = print(binaryExpression->right);
    return nullptr;
}

Expression* Printer::print(Expression* expr) { 
    return expr->accept(this);
}

Expression* Printer::VisitVariableExpression(VariableExpression* expression) { return nullptr; }

void Printer::VisitPrintStatement(PrintStatement* printStatement) { return; }
void Printer::VisitExpressionStatement(ExpressionStatement* ExpressionStatement) { return; }

void Printer::VisitVariableDeclarationStatement(VariableDeclarationStatement* variableDeclarationStatement) { return; }
void Printer::VisitFunctionDeclarationStatement(FunctionDeclarationStatement* functionDeclarationStatement) { return; }
void Printer::VisitReturnStatement(ReturnStatement* returnStatement) { return; }
Expression* Printer::VisitVariableAssignmentExpression(VariableAssignmentExpression* variableAssignmentExpression) { return nullptr; }

void Printer::VisitBlockStatement(BlockStatement* blockStatement) { return; }

void Printer::VisitIfStatement(IfStatement* ifStatement) { return; }
void Printer::VisitWhileStatement(WhileStatement* whileStatement) { return; }
void Printer::VisitForStatement(ForStatement* ForStatement) { return; }