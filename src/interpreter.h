#pragma once

#include "nodes.h"
#include "visitor.h"
#include "ASTprinter.h"
#include "environment.h"

class ReturnException : public std::exception {
public:
    ReturnException(Expression* expr);

    Expression* value;
};


class Interpreter : public Visitor {
public:
    Interpreter();
    void Interpret(std::vector<Statement*> statements);


    NumericLiteral* VisitNumericLiteralExpression(NumericLiteral* numericLiteral) override;
    StringLiteral* VisitStringLiteralExpression(StringLiteral* stringLiteral) override;
    BooleanLiteral* VisitBooleanLiteralExpression(BooleanLiteral* booleanLiteral) override;

    Expression* VisitGroupExpression(GroupExpression* groupExpression) override;
    Expression* VisitUnaryExpression(UnaryExpression* unaryExpression) override;
    Expression* VisitCallExpression(CallExpression* callExpression) override;
    Expression* VisitBinaryExpression(BinaryExpression* binaryExpression) override;

    Expression* VisitVariableExpression(VariableExpression* variableExpression) override;
    Expression* VisitVariableAssignmentExpression(VariableAssignmentExpression* variableAssignmentExpression) override;

    void VisitExpressionStatement(ExpressionStatement* expressionStatement) override;
    void VisitPrintStatement(PrintStatement* printStatement) override;
    void VisitVariableDeclarationStatement(VariableDeclarationStatement* variableStatement) override;
    void VisitFunctionDeclarationStatement(FunctionDeclarationStatement* functionDeclarationStatement) override;
    void VisitReturnStatement(ReturnStatement* returnStatement) override;
    void VisitBlockStatement(BlockStatement* blockStatement) override;
    void VisitIfStatement(IfStatement* ifStatement) override;
    void VisitWhileStatement(WhileStatement* whileStatement) override;
    void VisitForStatement(ForStatement* forStatement) override;


    Expression* evaluate(Expression* expr);
    void execute(Statement* stmt);
    void executeBlock(std::vector<Statement*> stmts, Environment* env);

public:
    Environment* globals;


private:
    Printer printer;
    Environment* environment;

    
};