


#include <vector>
#include <memory>
#include <iostream>
#include <string>

class Expression;
class Visitor;
class Interpreter;


#include "token.h"
#include "nodes.h"
#include "parserprinter.h"
#include "parser.h"

Parser::Parser(std::vector<Token>& tokens) {
    mTokens = tokens;
    mCurrent = 0;
	printer.setSource(tokens);
}

std::vector<Statement*> Parser::Parse() {
    std::vector<Statement*> statements;
    while (!eof()) {
        statements.push_back(declaration());
        enforce(TOKEN_TYPE::NEWLINE);
    }
	enforce(TOKEN_TYPE::END_OF_FILE);
    return statements;
}

Statement* Parser::declaration() {
    if (match(TOKEN_TYPE::SUBROUTINE)) {
        return functionDeclaration();
    }
    if (peek().GetType() == TOKEN_TYPE::IDENTIFIER && mTokens[mCurrent+1].GetType() == TOKEN_TYPE::ASSIGNMENT) {
        return variableDeclaration();
    }
    return statement();
}

Statement* Parser::variableDeclaration() {
    Token name = get();
    advance();
    Expression* val = expression();
    return new VariableDeclarationStatement(name, val);
}

Statement* Parser::functionDeclaration() {
    Token identifier = get();
    enforce(TOKEN_TYPE::LEFT_PAREN);
    std::vector<Token> parameters;
    while (peek().GetType() != TOKEN_TYPE::RIGHT_PAREN) {
        if (parameters.size() > 127) {
            throw std::runtime_error("Cannot have more than 127 arguments in function call\n"+identifier.GetLexeme());
        }
        Token param = get();
        parameters.push_back(param);
        if (peek().GetType() != TOKEN_TYPE::RIGHT_PAREN) {
            enforce(TOKEN_TYPE::COMMA);
        }
    }
    enforce(TOKEN_TYPE::RIGHT_PAREN);

    enforce(TOKEN_TYPE::NEWLINE);
    std::vector<Statement*> body = static_cast<BlockStatement*>(blockStatement(TOKEN_TYPE::ENDSUBROUTINE))->statements;
    return new FunctionDeclarationStatement(identifier, parameters, body);
}

Statement* Parser::returnStatement() {
    Expression* expr = nullptr;
    if (peek().GetType() != TOKEN_TYPE::NEWLINE) {
        expr = expression();
    }
	
	
    return new ReturnStatement(expr);
}

Statement* Parser::statement() {
	while (match(TOKEN_TYPE::NEWLINE)) {
		continue;
	}
    if (match(TOKEN_TYPE::IF)) {
        return ifStatement();
    }
    if (match(TOKEN_TYPE::WHILE)) {
        return whileStatement();
    }
    if (match(TOKEN_TYPE::PRINT)) {
        return printStatement();
    }
    if (match(TOKEN_TYPE::RETURN)) {
        return returnStatement();
    }
    if (match(TOKEN_TYPE::FOR)) {
        return forStatement();
    }
    return expressionStatement();
} 

Statement* Parser::ifStatement() {
    Expression* condition = expression();
    enforce(TOKEN_TYPE::THEN);
    enforce(TOKEN_TYPE::NEWLINE);
    
    std::vector<Statement*> branch;
    while ((peek().GetType() != TOKEN_TYPE::ELSE && peek().GetType() != TOKEN_TYPE::ENDIF) && !eof()) {
        branch.push_back(declaration());
        enforce(TOKEN_TYPE::NEWLINE);
    }

    std::vector<Statement*> elseBranch;
    if (match(TOKEN_TYPE::ELSE)) {
		while(peek().GetType() != TOKEN_TYPE::ENDIF && !eof()) {
			branch.push_back(declaration());
			enforce(TOKEN_TYPE::NEWLINE);
		}
    }
	match(TOKEN_TYPE::ENDIF);
	
    return new IfStatement(condition, new BlockStatement(branch), new BlockStatement(elseBranch));
}

Statement* Parser::whileStatement() {
    Expression* condition = expression();
    enforce(TOKEN_TYPE::DO);
    enforce(TOKEN_TYPE::NEWLINE);
    Statement* body = blockStatement(TOKEN_TYPE::ENDWHILE);
    return new WhileStatement(condition, body);
}

Statement* Parser::forStatement() {
    Token identifier = get();
    enforce(TOKEN_TYPE::ASSIGNMENT);
    Expression* from = expression();
    enforce(TOKEN_TYPE::TO);
    Expression* to = expression();
    enforce(TOKEN_TYPE::DO);
    enforce(TOKEN_TYPE::NEWLINE);
    Statement* body = blockStatement(TOKEN_TYPE::ENDFOR);
	enforce(TOKEN_TYPE::NEWLINE);
    return new ForStatement(identifier, from, to, body);
}

/*Statement* Parser::forStatement() {
    VariableDeclarationStatement* iter = dynamic_cast<VariableDeclarationStatement*>(variableDeclaration());
    enforce(TOKEN_TYPE::TO);
    enforce(TOKEN_TYPE::INTEGER);
    NumericLiteral* end = new NumericLiteral(stod(previous().GetLexeme()));
    enforce(TOKEN_TYPE::NEWLINE);
    Statement* body = statement();
    enforce(TOKEN_TYPE::ENDFOR);
    if (!eof()) { enforce(TOKEN_TYPE::NEWLINE); }
    return new ForStatement(iter->name, iter->val, end, body);

}*/

Statement* Parser::blockStatement(TOKEN_TYPE endingToken) {
    std::vector<Statement*> stmts;
    while (peek().GetType() != endingToken && !eof()) {
        stmts.push_back(declaration());
		enforce(TOKEN_TYPE::NEWLINE);
    }
    enforce(endingToken);
    return new BlockStatement(stmts);

}

Statement* Parser::printStatement() {
    Expression* expr = expression();
    return new PrintStatement(expr);
}

Statement* Parser::expressionStatement() {
    Expression* expr = expression();
	enforce(TOKEN_TYPE::NEWLINE);
    return expr;
}   





Expression* Parser::expression() {
    Expression* expr = assignment();
    
    return expr;
}

Expression* Parser::assignment() {
    Expression* expr = equality();
    if (peek().GetType() == TOKEN_TYPE::ASSIGNMENT) {
        Token equals = previous();
        advance();
        Expression* value = assignment();

        if (dynamic_cast<VariableExpression*>(expr) != nullptr) {
            Token name = ((VariableExpression*)expr)->name;
            return new VariableAssignmentExpression(name, expr);
        }

        //report a syntax error invalid assignment target
    }
    return expr;
}

Expression* Parser::equality() {
    Expression* expr = comparison();
    
    while (peek().GetType() == TOKEN_TYPE::NOT_EQUAL || peek().GetType() == TOKEN_TYPE::EQUAL) {
        Token operation = get();
        Expression* right = comparison();
        expr = new BinaryExpression(expr, operation, right);
    }
    return expr;
}

Expression* Parser::comparison() {
    Expression* expr = term();
    while (peek().GetType() == TOKEN_TYPE::LESS_EQ_THAN     ||
        peek().GetType() == TOKEN_TYPE::LESS_THAN        ||
        peek().GetType() == TOKEN_TYPE::GREATER_THAN     ||
        peek().GetType() == TOKEN_TYPE::GREATER_EQ_THAN) {
        Token operation = get();
        Expression* right = term();
        expr = new BinaryExpression(expr, operation, right);
    }
    return expr;
}

Expression* Parser::term() {
    Expression* expr = factor();
    while (peek().GetType() == TOKEN_TYPE::PLUS || peek().GetType() == TOKEN_TYPE::MINUS) {
        
        Token operation = get();
        Expression* right = factor();
        expr = new BinaryExpression(expr, operation, right);
    }
    return expr;
}

Expression* Parser::factor() {
    Expression* expr = unary();
    while (peek().GetType() == TOKEN_TYPE::SLASH || peek().GetType() == TOKEN_TYPE::STAR) {
        Token operation = get();
        Expression* right = unary();
        expr = new BinaryExpression(expr, operation, right);
    }
    return expr;
}

Expression* Parser::unary() {
    if (peek().GetType() == TOKEN_TYPE::EXCLAMATION || peek().GetType() == TOKEN_TYPE::MINUS) {
        Token operation = get();
        Expression* right = unary();
        return new UnaryExpression(operation, right);
    }
    return call();
}

Expression* Parser::call() {
    Expression* expr = primary();
    while (true) {
        if (peek().GetType() == TOKEN_TYPE::LEFT_PAREN) {
            expr = finishCall(expr);
        }
        else {
            break;
        }
    }

    return expr;
}

Expression* Parser::finishCall(Expression* callee) {
    std::vector<Expression*> args;
    advance();
    while (peek().GetType() != TOKEN_TYPE::RIGHT_PAREN) {
        if (args.size()>127) {
            //cannot have more than 127 args, throw an error
        }
        args.push_back(expression());
        if (peek().GetType() != TOKEN_TYPE::RIGHT_PAREN) {
            enforce(TOKEN_TYPE::COMMA);
        }
    }

    enforce(TOKEN_TYPE::RIGHT_PAREN);
    return new CallExpression(callee, args);
}

Expression* Parser::primary() {
    switch (peek().GetType())
    {
	case TOKEN_TYPE::INPUT:
		advance();
		return new InputExpression();
    case TOKEN_TYPE::FALSE:
        advance();
        return new BooleanLiteral(false);
    case TOKEN_TYPE::TRUE:
        advance();
        return new BooleanLiteral(true);
    case TOKEN_TYPE::IDENTIFIER:
        return new VariableExpression(get());
    case TOKEN_TYPE::INTEGER:
        return new NumericLiteral(Token(get()));
    case TOKEN_TYPE::REAL:
        return new NumericLiteral(Token(get()));
    case TOKEN_TYPE::STRING:
        return new StringLiteral(get().GetLexeme());
    case TOKEN_TYPE::LEFT_PAREN:
        advance();
        Expression* expr = expression();
        enforce(Token(TOKEN_TYPE::RIGHT_PAREN));
        return new GroupExpression(expr);
    }
    return nullptr;
}

Token Parser::previous() { return mTokens[mCurrent-1]; }
Token Parser::get() { 
    if (!eof()) {
        advance();
		printer.Print(mCurrent);
        return previous();  
    }
	printer.Print(mCurrent);
    return Token(TOKEN_TYPE::END_OF_FILE);
//    std::cout<<static_cast<int>(mTokens[mCurrent].GetType())<<std::endl;
}

bool Parser::match(TOKEN_TYPE type) {
	if (peek().GetType() == type) {
		advance();
		return true;
	}
	return false;
}
Token Parser::peek() { return mTokens[mCurrent]; }
void Parser::advance() { mCurrent++; printer.Print(mCurrent); }

void Parser::enforce(Token token) {
    if (mTokens[mCurrent].GetType() != token.GetType()) {
		if (token.GetType() == TOKEN_TYPE::NEWLINE && mTokens[mCurrent].GetType() == TOKEN_TYPE::END_OF_FILE) { return; }
        std::cout<<"Syntax Error: Expected: "<<token.GetTypeString()<< " but got: "<< mTokens[mCurrent].GetTypeString()<< " instead"<<std::endl;
		std::cout<<"\tline: "<<mTokens[mCurrent].GetLine()<<std::endl;
		throw std::runtime_error("Quitting...");
    }
	advance();
}
bool Parser::eof() { 
    if (mCurrent >= mTokens.size()-1) { return true; }
    return false;
}