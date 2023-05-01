#pragma once


enum class TOKEN_TYPE
{
    IF=0, ELSE, ELSEIF, ENDIF,
    FOR, ENDFOR, WHILE, ENDWHILE, TO, STEP,
    RETURN, CONTINUE, BREAK,
    AND, OR, NOT, TRUE, FALSE,
    SUBROUTINE, ENDSUBROUTINE, CLASS, ENDCLASS, PRINT, INPUT,
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_SQUARE, RIGHT_SQUARE,
    PLUS, MINUS, STAR, SLASH,
	STAR_STAR, PLUS_PLUS, MINUS_MINUS, EQUAL_EQUAL, SLASH_SLASH,
    DOT, COMMA, COLON, SEMICOLON, EQUAL,
    EXCLAMATION, NOT_EQUAL,
    ASSIGNMENT,
    GREATER_THAN, GREATER_EQ_THAN,
    LESS_THAN, LESS_EQ_THAN,
    IDENTIFIER, STRING, INTEGER, REAL, CHAR,
    NEWLINE,
    END_OF_FILE, ERROR, SPACE, THEN, DO                 
};

class Token
{
public:
    Token(TOKEN_TYPE tag, std::string lexeme);
    Token(int line, TOKEN_TYPE tag, std::string lexeme);
	Token(TOKEN_TYPE tag);
	Token(int line, TOKEN_TYPE tag);
    
    Token() {};
    Token(const Token& t1);


    TOKEN_TYPE GetType() const;
    int GetLine() const;

    std::string GetLexeme();

    friend std::ostream& operator<<(std::ostream& os, const Token& token);

private:
    TOKEN_TYPE mType;
    std::string mLexeme;
    int mLine;
};






