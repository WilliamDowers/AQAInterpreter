#include <string>
#include <vector>

class Expression;
class Interpreter;
class Visitor;
class Function;


#include "token.h"
#include "nodes.h"
#include "visitor.h"
#include "environment.h"
#include "interpreter.h"
#include "callable.h"

Function::Function(FunctionDeclarationStatement* functionDeclarationStatement) {
    declaration = functionDeclarationStatement;
}


Expression* Function::call(Interpreter* interpreter, std::vector<Expression*> args) {
    Environment* environment = new Environment(interpreter->globals);
    for (int i = 0; i < declaration->parameters.size(); i++) {
        if (dynamic_cast<BooleanLiteral*>(args[i]) != nullptr) {
            environment->Declare(declaration->parameters[i].GetLexeme(), dynamic_cast<BooleanLiteral*>(args[i])->val);
            continue;
        }
        if (dynamic_cast<StringLiteral*>(args[i]) != nullptr) {
            environment->Declare(declaration->parameters[i].GetLexeme(), dynamic_cast<StringLiteral*>(args[i])->val);
            continue;
        }
        if (dynamic_cast<NumericLiteral*>(args[i]) != nullptr) {
            environment->Declare(declaration->parameters[i].GetLexeme(), dynamic_cast<NumericLiteral*>(args[i])->val);
            continue;
        }
    }
    try {
        interpreter->executeBlock(declaration->body, environment);

    } catch (ReturnException returnValue) {
        return returnValue.value;
    }
    return nullptr;
}

int Function::Arity() {
    return declaration->parameters.size();
}

