#include <iostream>
#include <vector>

class Visitor;
class Interpreter;
class Printer;
class Function;

#include "token.h"
#include "lexer.h"
#include "parserprinter.h"
#include "nodes.h"
#include "parser.h"
#include "visitor.h"
#include "interpreter.h"
#include "ASTprinter.h"
#include "environment.h"
#include "callable.h"



int main(int argc, char** argv)
{

    if (argc != 2)
    {
        std::cerr<<"Improper Usage- Interpreter.exe [filename]"<<std::flush;
        return -1;
    }

    Lexer lexer(argv[1]); // construct a lexical analyser object that can then be passed by reference to the parser and interacted via the Token lexer::GetNextToken() method
    std::vector<Token> vectors;
    lexer.GetAllTokens(vectors);
    std::cout<<" ### LEXICAL ANALYSIS ###\n"<<std::endl;

    for (Token token : vectors) {
        if (token.GetType() == (TOKEN_TYPE::SPACE)) { continue; }
		std::cout<<token.GetTypeString()<<" \t\t"<<token.GetLexeme()<<std::endl;
        if (token.GetType() == (TOKEN_TYPE::END_OF_FILE)) { break; }
    }
    std::cout<<"\n\n ### GENERATING ABSTRACT SYNTAX TREE ###\n"<<std::endl;

    Parser parser(vectors);
    std::vector<Statement*> statements = parser.Parse();
    std::cout<<"\nast was generated"<<std::endl;
    

    std::cout<<"\n\n\n###\t\tMAIN PROGRAM\t\t###\n\n\n";
    Interpreter interpreter;
    interpreter.Interpret(statements);






}