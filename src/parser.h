#pragma once

class Parser {
public:
    Parser(std::vector<Token>& tokens);
    std::vector<Statement*> Parse();
private:
    Expression* expression();
    Expression* equality();
    Expression* assignment();
    Expression* comparison();
    Expression* term();
    Expression* factor();
    Expression* unary();
    Expression* call();
    Expression* finishCall(Expression* callee);
    Expression* primary();

    Statement* declaration();
    Statement* statement();
    Statement* variableDeclaration();
    Statement* functionDeclaration();
    Statement* returnStatement();
    Statement* printStatement();
    Statement* expressionStatement();
    Statement* blockStatement(TOKEN_TYPE endingToken);
    Statement* ifStatement();
    Statement* whileStatement();
    Statement* forStatement();

    Token previous();
    Token get();
    Token peek();
    void advance();

    void enforce(Token token);
    bool eof();
    bool end_of_file;
    std::vector<Token> mTokens;
    int mCurrent;
};