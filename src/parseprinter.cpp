
#include <iostream>
#include <string>
#include <vector>

#include "token.h"
#include "lexer.h"
#include "parserprinter.h"


void ParserPrinter::Print(int pos) {
	
	for (int i = 0; i < source.size(); i++) {
		if (i >= pos) {
			std::cout<< '['<<source[i].GetTypeString()<<", "<<source[i].GetLexeme()<<"]\t";
		}
		else {
			std::cout<< '<'<<source[i].GetTypeString()<<", "<<source[i].GetLexeme()<<">\t";
		}
		if (source[i].GetType() == TOKEN_TYPE::NEWLINE) {
			std::cout<<'\n';
		}
		if (source[i].GetType() == TOKEN_TYPE::END_OF_FILE) {
			std::cout<<"\n\n\n";
		}
	}
}

void ParserPrinter::setSource(std::vector<Token>& src) {
	source = src;
}