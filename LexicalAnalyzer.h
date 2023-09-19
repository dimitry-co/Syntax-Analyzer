#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <fstream>
#include <map>
#include <string>
#include <ctype.h>
#include <stdio.h>

#include "Token.h"
#include "TokenCodes.h"

using namespace std;

class LexicalAnalyzer
{
  private:
    ifstream *sourceCodeFile;
    string* lexeme;
    string currChars;
    string line;
    TokenCodes newtc;
    char nextChar;
    char whiteSp;
    int charClass;
    char currChar;
    static string lexList[49];
    std::map<std::string, int> tokenMap;
    int currentTokenPosition;
    int currentIndOfLine;
    int end_of_line;

    void getTokenPosAndCharClass();
    void getFirstChar();
    void getCharClass();
    void findLettOrResWord();
    void getNum();
    void check_Valid_Single_or_Double_Chars();
    void getNextChar();

  public:
    LexicalAnalyzer(ifstream*);
    Token* getNextToken();
    int getCurrentTokenPosition();
};

#endif