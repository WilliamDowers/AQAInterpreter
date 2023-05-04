#pragma once

class ParserPrinter {
public:

	void setSource(std::vector<Token>& src);

	void Print(int pos);

private:
	std::vector<Token> source;
};