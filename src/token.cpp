#include <iostream>
#include <string>

#include "token.h"

Token::Token(TOKEN_TYPE type, std::string lexeme) {
    mType = type;
    mLexeme = lexeme;
}

Token::Token(int line, TOKEN_TYPE type, std::string lexeme) {
    mLine = line; 
    mType = type;
    mLexeme = lexeme;
}

Token::Token(int line, TOKEN_TYPE type) {
    mLine = line;
    mType = type;
}

Token::Token(TOKEN_TYPE type) { mType = type; }

Token::Token(const Token& t1) {
    mType = t1.mType;
    mLexeme = t1.mLexeme;
}


TOKEN_TYPE Token::GetType() const { return mType; }

/*std::string Token::GettypeString()  {
std::string typebinds[] = {"IF",
"ELSE",
"ELSEIF",
"ENDIF",
"FOR",
"ENDFOR",
"WHILE",
"ENDWHILE",
"TO",
"STEP",
"RETURN",
"CONTINUE",
"BREAK",
"AND",
"OR",
"NOT",
"TRUE",
"FALSE",
"SUBROUTINE",
"ENDSUBROUTINE",
"CLASS",
"ENDCLASS",
"PRINT",
"INPUT",
"LEFT_PAREN",
"RIGHT_PAREN",
"LEFT_BRACE",
"RIGHT_BRACE",
"LEFT_SQUARE",
"RIGHT_SQUARE",
"PLUS",
"MINUS",
"STAR",
"SLASH",
"STAR_STAR",
"PLUS_PLUS",
"MINUS_MINUS",
"EQUAL_EQUAL",
"SLASH_SLASH",
"DOT",
"COMMA",
"COLON",
"SEMICOLON",
"EQUAL",
"EXCLAMATION",
"NOT_EQUAL",
"ASSIGNMENT",
"GREATER_THAN",
"GREATER_EQ_THAN",
"LESS_THAN",
"LESS_EQ_THAN",
"IDENTIFIER",
"STRING",
"INTEGER",
"REAL",
"CHAR",
"NEWLINE",
"END_OF_FILE",
"ERROR",
""};
return typebinds[static_cast<int>(mType)];
}
std::ostream& operator<<(std::ostream& os, const Token& token) { 
    os << '<' << token.Gettype() << ',' << token.mLexeme << '>'; 
    return os; 
}*/

std::string Token::GetLexeme() { return mLexeme; }
int Token::GetLine() const { return mLine; }