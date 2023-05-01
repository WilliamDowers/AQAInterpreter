#include <unordered_map>
#include <fstream>
#pragma once


class Lexer
{
public:
	Lexer(const std::string &filePath);
	~Lexer();
	Token GetNextToken();
	void GetAllTokens(std::vector<Token>& tokens);
private:
	void InitKeyWords();

private:
	std::ifstream mSrc;
	std::unordered_map<std::string, TOKEN_TYPE> mKeyWords;
	int mCurrent;
	int line;
};