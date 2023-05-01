#pragma once

#include "nodes.h"

class Visitor{
public:
    virtual NumericLiteral* VisitNumericLiteralExpression(NumericLiteral* numericLiteral) = 0;
    virtual StringLiteral* VisitStringLiteralExpression(StringLiteral* stringLiteral) = 0;
    virtual BooleanLiteral* VisitBooleanLiteralExpression(BooleanLiteral* booleanLiteral) = 0;

    virtual Expression* VisitGroupExpression(GroupExpression* groupExpression) = 0;
    virtual Expression* VisitUnaryExpression(UnaryExpression* unaryExpression) = 0;
    virtual Expression* VisitCallExpression(CallExpression* callExpression) = 0;
    virtual Expression* VisitBinaryExpression(BinaryExpression* binaryExpression) = 0;

    virtual Expression* VisitVariableExpression(VariableExpression* variableExpression) = 0;
    virtual Expression* VisitVariableAssignmentExpression(VariableAssignmentExpression* variableAssignmentExpression) = 0;

    virtual void VisitExpressionStatement(ExpressionStatement* expressionStatement) = 0;
    virtual void VisitPrintStatement(PrintStatement* printStatement) = 0;
    virtual void VisitVariableDeclarationStatement(VariableDeclarationStatement* variableStatement) = 0;
    virtual void VisitFunctionDeclarationStatement(FunctionDeclarationStatement* functionDeclaration) = 0;
    virtual void VisitReturnStatement(ReturnStatement* returnStatement) = 0;
    virtual void VisitBlockStatement(BlockStatement* blockStatement) = 0;
    virtual void VisitIfStatement(IfStatement* ifStatement) = 0;
    virtual void VisitWhileStatement(WhileStatement* whileStatement) = 0;
    virtual void VisitForStatement(ForStatement* forStatement) = 0;


};  