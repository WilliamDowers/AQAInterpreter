#pragma once



class Callable {
public:
    Callable() {};
    virtual Expression* call(Interpreter* interpreter, std::vector<Expression*> args) = 0;

    virtual int Arity() = 0;
};

class Function : public Callable {
public:
    Function(FunctionDeclarationStatement* functionDeclarationStatement);
    Expression* call(Interpreter* interpreter, std::vector<Expression*> args) override;
    int Arity() override;
private:
    FunctionDeclarationStatement* declaration;
    Environment* closure;


};