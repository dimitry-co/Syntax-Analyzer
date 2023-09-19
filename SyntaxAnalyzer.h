#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "LexicalAnalyzer.h"
#include "Token.h"
#include "TokenCodes.h"
#include <vector>
#include <string>

using namespace std;

class SyntaxAnalyzer
{
  private:
    LexicalAnalyzer *la;
    Token *nextToken;
    TokenCodes nextTokenCode;
    vector <TokenCodes> token_errors;

    void PROGRAM();
    void Decpart();
    void Declarations();
    void Declaration();
    void Identlist();
    void Compstmt();
    void Seqofstmt();
    void Block();
    void Statement();
    void StatementFor();
    void StatementIdent();
    void StatementIf();
    void StatementRepeat();
    void StatementWhile();
    void StatementReadOrWrite();
    void Expression();
    void Simpexpr();
    void Term();
    void Factor();
    void Primary();
    void getNextInputToken();
    void error();

  public:
    SyntaxAnalyzer(LexicalAnalyzer*);
    void Program();
};

#endif
