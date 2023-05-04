#pragma once


#include "visitor.h"

class Printer : public Visitor {
public:
    void Print(Expression* expression);


    NumericLiteral* VisitNumericLiteralExpression(NumericLiteral* numericLiteral) override;
    StringLiteral* VisitStringLiteralExpression(StringLiteral* stringLiteral) override;
    BooleanLiteral* VisitBooleanLiteralExpression(BooleanLiteral* booleanLiteral) override;

    Expression* VisitGroupExpression(GroupExpression* groupExpression) override;
    Expression* VisitUnaryExpression(UnaryExpression* unaryExpression) override;
    Expression* VisitCallExpression(CallExpression* callExpression) override;
    Expression* VisitBinaryExpression(BinaryExpression* binaryExpression) override;

    Expression* VisitVariableExpression(VariableExpression* variableExpression) override;
    Expression* VisitVariableAssignmentExpression(VariableAssignmentExpression* variableAssignmentExpression) override;

	Expression* VisitInputExpression(InputExpression* inputExpression) override;

    void VisitExpressionStatement(ExpressionStatement* expressionStatement) override;
    void VisitPrintStatement(PrintStatement* printStatement) override;
    void VisitVariableDeclarationStatement(VariableDeclarationStatement* variableDeclarationStatement) override;
    void VisitFunctionDeclarationStatement(FunctionDeclarationStatement* functionDeclarationStatement) override;
    void VisitReturnStatement(ReturnStatement* returnStatement) override;
    void VisitBlockStatement(BlockStatement* blockStatement) override;
    void VisitIfStatement(IfStatement* ifStatement) override;
    void VisitWhileStatement(WhileStatement* whileStatement) override;
    void VisitForStatement(ForStatement* ForStatement) override;


    Expression* print(Expression* expr);
};




