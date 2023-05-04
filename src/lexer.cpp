
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>


#include "token.h"
#include "lexer.h"



Lexer::Lexer(const std::string &filePath) {
    mSrc.open(filePath);
    line = 0;
    InitKeyWords();
}

Lexer::~Lexer() {
    mSrc.close();
}


Token Lexer::GetNextToken() {
    
    switch(mSrc.get()) {
        case ' ': return GetNextToken();
		case '\t': return GetNextToken();
        case '(': return Token(line, TOKEN_TYPE::LEFT_PAREN);
        case ')': return Token(line, TOKEN_TYPE::RIGHT_PAREN);
        case '{': return Token(line, TOKEN_TYPE::LEFT_BRACE);
        case '}': return Token(line, TOKEN_TYPE::RIGHT_BRACE);
        case '[': return Token(line, TOKEN_TYPE::LEFT_SQUARE);
        case ']': return Token(line, TOKEN_TYPE::RIGHT_SQUARE);
        case '.': return Token(line, TOKEN_TYPE::DOT);
        case ',': return Token(line, TOKEN_TYPE::COMMA);
        case ':': return Token(line, TOKEN_TYPE::COLON);
        case ';': return Token(line, TOKEN_TYPE::SEMICOLON);
        case '=': return Token(line, TOKEN_TYPE::EQUAL);
        case '\n': 
            line++;
            return Token(line, TOKEN_TYPE::NEWLINE);

        case '+': 
            if (mSrc.peek() == '+') { mSrc.ignore(); return Token(line, TOKEN_TYPE::PLUS_PLUS); }
            else { return Token(line, TOKEN_TYPE::PLUS); }
        case '-':
            if (mSrc.peek() == '-') { mSrc.ignore(); return Token(line, TOKEN_TYPE::MINUS_MINUS); }
            else { return Token(line, TOKEN_TYPE::MINUS); }
        case '*':
            if (mSrc.peek() == '*') { mSrc.ignore(); return Token(line, TOKEN_TYPE::STAR_STAR); }
            else { return Token(line, TOKEN_TYPE::STAR); }
        case '/':
            if (mSrc.peek() == '/') { mSrc.ignore(); return Token(line, TOKEN_TYPE::SLASH_SLASH); }
            else { return Token(line, TOKEN_TYPE::SLASH); }
        
        case '<':
            if (mSrc.peek() == '-') { mSrc.ignore(); return Token(line, TOKEN_TYPE::ASSIGNMENT); }
            else if (mSrc.peek() == '=') { mSrc.ignore(); return Token(line, TOKEN_TYPE::LESS_EQ_THAN); }
            else { return Token(line, TOKEN_TYPE::LESS_THAN); }
        case '>':
            if (mSrc.peek() == '=') { mSrc.ignore(); return Token(line, TOKEN_TYPE::GREATER_EQ_THAN); }
            else { return Token(line, TOKEN_TYPE::GREATER_THAN); }
        case '!':
            if (mSrc.peek() == '=') { mSrc.ignore(); return Token(line, TOKEN_TYPE::NOT_EQUAL); }
            else { return Token(line, TOKEN_TYPE::NOT); }
        
        
        

        case '"':
        {
            std::string lexeme;
            while (mSrc.peek() != '"') { lexeme.push_back(mSrc.get()); }
            mSrc.ignore();
            return Token(line, TOKEN_TYPE::STRING, lexeme);
        }

        default:
            mSrc.unget();
            if (isdigit(mSrc.peek())) {
                std::string lexeme;
                while (isdigit(mSrc.peek())) { lexeme.push_back(mSrc.get()); }
                if (mSrc.peek() != '.') { return Token(line, TOKEN_TYPE::INTEGER, lexeme); }
                mSrc.ignore();
                lexeme.push_back('.');
                while (isdigit(mSrc.peek())) { lexeme.push_back(mSrc.get()); }
                return Token(line, TOKEN_TYPE::REAL, lexeme);
            }

            if (isalpha(mSrc.peek())) {
                std::string lexeme;
                while (isalnum(mSrc.peek())) { lexeme.push_back(mSrc.get()); }
                Token(line, TOKEN_TYPE::IDENTIFIER, lexeme);
                if (mKeyWords.contains(lexeme))	{ return Token(mKeyWords[lexeme]); }
                return Token(line, TOKEN_TYPE::IDENTIFIER, lexeme);
            }
    };
    
}



void Lexer::GetAllTokens(std::vector<Token>& tokens) {
    while (mSrc.good())
        tokens.push_back(GetNextToken());
	
	tokens.push_back(Token(line, TOKEN_TYPE::END_OF_FILE));
}

void Lexer::InitKeyWords() {
    mKeyWords["IF"] = TOKEN_TYPE::IF;
    mKeyWords["ELSE"] = TOKEN_TYPE::ELSE;
    mKeyWords["ELSEIF"] = TOKEN_TYPE::ELSEIF;
    mKeyWords["ENDIF"] = TOKEN_TYPE::ENDIF;
    mKeyWords["THEN"] = TOKEN_TYPE::THEN;

    mKeyWords["FOR"] = TOKEN_TYPE::FOR;
    mKeyWords["STEP"] = TOKEN_TYPE::STEP;
    mKeyWords["TO"] = TOKEN_TYPE::TO;
    mKeyWords["ENDFOR"] = TOKEN_TYPE::ENDFOR;
    mKeyWords["DO"] = TOKEN_TYPE::DO;

    mKeyWords["WHILE"] = TOKEN_TYPE::WHILE;
    mKeyWords["ENDWHILE"] = TOKEN_TYPE::ENDWHILE;

    mKeyWords["RETURN"] = TOKEN_TYPE::RETURN;
    mKeyWords["CONTINUE"] = TOKEN_TYPE::CONTINUE;
    mKeyWords["BREAK"] = TOKEN_TYPE::BREAK;

    mKeyWords["AND"] = TOKEN_TYPE::AND;
    mKeyWords["OR"] = TOKEN_TYPE::OR;

    mKeyWords["NOT"] = TOKEN_TYPE::NOT;
    mKeyWords["TRUE"] = TOKEN_TYPE::TRUE;
    mKeyWords["FALSE"] = TOKEN_TYPE::FALSE;

    mKeyWords["SUBROUTINE"] = TOKEN_TYPE::SUBROUTINE;
    mKeyWords["ENDSUBROUTINE"] = TOKEN_TYPE::ENDSUBROUTINE;

    mKeyWords["CLASS"] = TOKEN_TYPE::CLASS;
    mKeyWords["ENDCLASS"] = TOKEN_TYPE::ENDCLASS;

    mKeyWords["PRINT"] = TOKEN_TYPE::PRINT;
    mKeyWords["INPUT"] = TOKEN_TYPE::INPUT;
}