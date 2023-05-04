#pragma once

class Expression;
class Printer;

class Statement {
public: 
    Statement();
    virtual Statement* accept(Visitor* Interpreter)=0;
};

class PrintStatement : public Statement {
public:
    PrintStatement(Expression* expression);
    
    Statement* accept(Visitor* interpreter) override;
    Expression* expr;
};



class ExpressionStatement : public Statement {
public:
    ExpressionStatement(Expression* expression);

    Statement* accept(Visitor* interpreter) override;
    Expression* expr;
};

class VariableDeclarationStatement : public Statement {
public:
    VariableDeclarationStatement(Token identifier, Expression* value);

    Statement* accept(Visitor* interpreter) override;
    Token name;
    Expression* val;
};

class FunctionDeclarationStatement : public Statement {
public:
    FunctionDeclarationStatement(Token name, std::vector<Token> params, std::vector<Statement*> bdy);

    Statement* accept(Visitor* interpreter) override;
    Token identifier;
    std::vector<Token> parameters;
    std::vector<Statement*> body;
};


class ReturnStatement : public Statement {
public:
    ReturnStatement(Expression* expr);
    Statement* accept(Visitor* interpreter) override;

    Expression* returnValue;
};

class BlockStatement : public Statement {
public:
    BlockStatement(std::vector<Statement*> stmts);
    Statement* accept(Visitor* interpreter) override;

    std::vector<Statement*> statements;
};

class IfStatement : public Statement {
public:
    IfStatement(Expression* cond, Statement* brnch, Statement* elseBrnch);

    Statement* accept(Visitor* interpreter) override;

    Expression *condition;
    Statement *branch, *elseBranch;
};

class WhileStatement : public Statement {
public:
    WhileStatement(Expression* cond, Statement* bdy);

    Statement* accept(Visitor* interpreter) override;

    Expression* condition;
    Statement* body;
};

class ForStatement : public Statement {
public:
    ForStatement(Token it, Expression* begin, Expression* finish, Statement* bdy);

    Statement* accept(Visitor* interpreter) override;

    Token iterator;
    Expression* start;
    Expression* end;
    Statement* body;
};


class Expression : public Statement {
public:
    Expression();
    virtual Expression* accept(Visitor* interpreter)=0;
};

class BooleanLiteral : public Expression {
public:
    BooleanLiteral(bool mVal);
    Expression* accept(Visitor* interpreter);

    bool val;
};

class StringLiteral : public Expression {
public:
    StringLiteral(std::string _val);
    Expression* accept(Visitor* interpreter) override;
    std::string val;
};


class NumericLiteral : public Expression {
public:
    NumericLiteral(Token _num);
    NumericLiteral(double _val);
    Expression* accept(Visitor* interpreter) override;

    double val;
};

class GroupExpression : public Expression {
public:
    GroupExpression(Expression* _expr);
    ~GroupExpression() ;
    
    Expression* accept(Visitor* interpreter) override;

    Expression* expr;
};

class UnaryExpression : public Expression {
public:
    UnaryExpression(Token _operation, Expression* _expr);
    ~UnaryExpression();
    Expression* accept(Visitor* interpreter) override;

    Token operation;
    Expression* expr;
};

class CallExpression : public Expression {
public:
    CallExpression(Expression* callee, std::vector<Expression*> args);

    Expression* accept(Visitor* interpreter) override;

    Expression* callee;
    std::vector<Expression*> args;
};

class BinaryExpression : public Expression {
public:
    BinaryExpression(Expression* _left, Token _operation, Expression* _right);
    ~BinaryExpression();
    Expression* accept(Visitor* interpreter) override;


    Expression* left, *right;
    Token operation;
};

class VariableExpression : public Expression {
public:
    VariableExpression(Token identifier);

    Expression* accept(Visitor* interpreter) override;

    Token name;
};

class InputExpression : public Expression {
public:
	InputExpression();

	Expression* accept(Visitor* Interpreter) override;

	
};

class VariableAssignmentExpression : public Expression {
public:
    VariableAssignmentExpression(Token identifier, Expression* expr);
    Expression* accept(Visitor* interpreter) override;

    Token name;
    Expression* val;
};

