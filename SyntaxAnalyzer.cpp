#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "Token.h"
#include "TokenCodes.h"
#include <vector>
#include <string>

using namespace std;

SyntaxAnalyzer::SyntaxAnalyzer(LexicalAnalyzer *l)
{
  la = l;
}

void SyntaxAnalyzer::Program()
{
  getNextInputToken();
  if (nextTokenCode == PROGSYM)
  {
    getNextInputToken();
    if (nextTokenCode == IDENT)
    {
      getNextInputToken();
      if (nextTokenCode == SEMICOLON)
      {
        getNextInputToken();
        Decpart();
        Compstmt();
        if (nextTokenCode == PERIOD)
        {
          getNextInputToken();
          if (nextTokenCode == EOI)
          {
            cout << "The program is syntactically correct.";
          }
          else
          {
            cout << "Error reading the program.";
          }
        }
        else
        {
          token_errors.push_back(PERIOD);
          error();
        }
      }
      else
      {
        token_errors.push_back(SEMICOLON);
        error();
      }
    }
    else
    {
      token_errors.push_back(IDENT);
      error();
    }
  }
  else
  {
    token_errors.push_back(PROGSYM);
    error();
  }
}

void SyntaxAnalyzer::Decpart()
{
  if (nextTokenCode == VARSYM)
  {
    getNextInputToken();
    Declarations();
  }
  else
    return;
}

void SyntaxAnalyzer::Declarations()
{
  Declaration();
  while (nextTokenCode == IDENT)
  {
    Declaration();
  }
}

void SyntaxAnalyzer::Declaration()
{
  Identlist();
  if (nextTokenCode == COLON)
  {
    getNextInputToken();
    if (nextTokenCode == BOOLSYM || nextTokenCode == INTSYM || nextTokenCode == REALSYM)
    {
      getNextInputToken();
      if (nextTokenCode == SEMICOLON)
      {
        getNextInputToken();
      }
      else
      {
        token_errors.push_back(SEMICOLON);
        error();
      }
    }
    else
    {
      token_errors.push_back(BOOLSYM);
      token_errors.push_back(INTSYM);
      token_errors.push_back(REALSYM);
      error();
    }
  }
  else
  {
    token_errors.push_back(COLON);
    error();
  }
}
void SyntaxAnalyzer::Identlist()
{
  if (nextTokenCode == IDENT)
  {
    getNextInputToken();
    while (nextTokenCode == COMMA)
    {
      getNextInputToken();
      Identlist();
    }
  }
  else
  {
    token_errors.push_back(IDENT);
    error();
  }
}
void SyntaxAnalyzer::Compstmt()
{
  if (nextTokenCode == BEGINSYM)
  {
    getNextInputToken();
    Seqofstmt();
    if (nextTokenCode == ENDSYM)
    {
      getNextInputToken();
    }
    else
    {
      token_errors.push_back(ENDSYM);
      error();
    }
  }
  else
  {
    token_errors.push_back(BEGINSYM);
    error();
  }
}
void SyntaxAnalyzer::Seqofstmt()
{
  Statement();
  while (nextTokenCode == SEMICOLON)
  {
    getNextInputToken();
    Statement();
  }
}

void SyntaxAnalyzer::Block()
{
  if (nextTokenCode == BEGINSYM)
  {
    Compstmt();
  }
  else if (nextTokenCode == FORSYM || nextTokenCode == IDENT || nextTokenCode == IFSYM ||
           nextTokenCode == REPEATSYM || nextTokenCode == WHILESYM || nextTokenCode == READSYM ||
           nextTokenCode == READLNSYM || nextTokenCode == WRITESYM || nextTokenCode == WRITELNSYM)
  {
    Statement();
  }
  else
  {
    token_errors.push_back(BEGINSYM);
    token_errors.push_back(FORSYM);
    token_errors.push_back(IDENT);
    token_errors.push_back(IFSYM);
    token_errors.push_back(REPEATSYM);
    token_errors.push_back(WHILESYM);
    token_errors.push_back(READSYM);
    token_errors.push_back(READLNSYM);
    token_errors.push_back(WRITESYM);
    token_errors.push_back(WRITELNSYM);
    error();
  }
}
void SyntaxAnalyzer::Statement()
{
  if (nextTokenCode == FORSYM)
  {
    getNextInputToken();
    StatementFor();
  }
  else if (nextTokenCode == IDENT)
  {
    getNextInputToken();
    StatementIdent();
  }
  else if (nextTokenCode == IFSYM)
  {
    getNextInputToken();
    StatementIf();
  }
  else if (nextTokenCode == REPEATSYM)
  {
    getNextInputToken();
    StatementRepeat();
  }
  else if (nextTokenCode == WHILESYM)
  {
    getNextInputToken();
    StatementWhile();
  }
  else if (nextTokenCode == READSYM || nextTokenCode == READLNSYM || nextTokenCode == WRITESYM || nextTokenCode == WRITELNSYM)
  {
    getNextInputToken();
    StatementReadOrWrite();
  }
  else
  {
    token_errors.push_back(FORSYM);
    token_errors.push_back(IDENT);
    token_errors.push_back(IFSYM);
    token_errors.push_back(REPEATSYM);
    token_errors.push_back(WHILESYM);
    token_errors.push_back(READSYM);
    token_errors.push_back(READLNSYM);
    token_errors.push_back(WRITESYM);
    token_errors.push_back(WRITELNSYM);
    error();
  }
}

void SyntaxAnalyzer::StatementFor()
{
  if (nextTokenCode == IDENT)
  {
    getNextInputToken();
    if (nextTokenCode == BECOMES)
    {
      getNextInputToken();
      Expression();
      if (nextTokenCode == TOSYM || nextTokenCode == DOWNTOSYM)
      {
        getNextInputToken();
        Expression();
        if (nextTokenCode == DOSYM)
        {
          getNextInputToken();
          Block();
        }
        else
        {
          token_errors.push_back(DOSYM);
          error();
        }
      }
      else
      {
        token_errors.push_back(TOSYM);
        token_errors.push_back(DOWNTOSYM);
        error();
      }
    }
    else
    {
      token_errors.push_back(BECOMES);
      error();
    }
  }
  else
  {
    token_errors.push_back(IDENT);
    error();
  }
}

void SyntaxAnalyzer::StatementIdent()
{
  if (nextTokenCode == BECOMES)
  {
    getNextInputToken();
    Expression();
  }
  else
  {
    token_errors.push_back(BECOMES);
    error();
  }
}

void SyntaxAnalyzer::StatementIf()
{
  Expression();
  if (nextTokenCode == THENSYM)
  {
    getNextInputToken();
    Block();
    if (nextTokenCode == ELSESYM)
    {
      getNextInputToken();
      Block();
    }
  }
  else
  {
    token_errors.push_back(THENSYM);
    error();
  }
}

void SyntaxAnalyzer::StatementRepeat()
{
  Seqofstmt();
  if (nextTokenCode == UNTILSYM)
  {
    getNextInputToken();
    Expression();
  }
  else
  {
    token_errors.push_back(UNTILSYM);
    error();
  }
}

void SyntaxAnalyzer::StatementWhile()
{
  Expression();
  if (nextTokenCode == DOSYM)
  {
    getNextInputToken();
    Block();
  }
  else
  {
    token_errors.push_back(DOSYM);
    error();
  }
}
void SyntaxAnalyzer::StatementReadOrWrite()
{
  if (nextTokenCode == LPAREN)
  {
    getNextInputToken();
    Identlist();
    if (nextTokenCode == RPAREN)
    {
      getNextInputToken();
    }
    else
    {
      token_errors.push_back(RPAREN);
      error();
    }
  }
  else
  {
    token_errors.push_back(LPAREN);
    error();
  }
}

void SyntaxAnalyzer::Expression()
{
  Simpexpr();
  if (nextTokenCode == EQL || nextTokenCode == NEQ || nextTokenCode == LSS || nextTokenCode == LEQ || nextTokenCode == GTR || nextTokenCode == GEQ)
  {
    Simpexpr();
  }
}

void SyntaxAnalyzer::Simpexpr()
{
  Term();
  while (nextTokenCode == PLUS || nextTokenCode == MINUS || nextTokenCode == ORSYM)
  {
    getNextInputToken();
    Term();
  }
}

void SyntaxAnalyzer::Term()
{
  Factor();
  while (nextTokenCode == TIMES || nextTokenCode == SLASH || nextTokenCode == DIVSYM || nextTokenCode == MODSYM || nextTokenCode == ANDSYM)
  {
    getNextInputToken();
    Factor();
  }
}
void SyntaxAnalyzer::Factor()
{

  if (nextTokenCode == NOTSYM)
  {
    getNextInputToken();
    Primary();
  }
  else
  {
    Primary();
  }
}

void SyntaxAnalyzer::Primary()
{
  if (nextTokenCode == LPAREN)
  {
    getNextInputToken();
    Expression();
  }
  else if (nextTokenCode == IDENT || nextTokenCode == NUMLIT || nextTokenCode == TRUESYM || nextTokenCode == FALSESYM)
  {
    getNextInputToken();
  }
  else
  {
    token_errors.push_back(IDENT);
    token_errors.push_back(NUMLIT);
    token_errors.push_back(TRUESYM);
    token_errors.push_back(FALSESYM);
    error();
  }
}

void SyntaxAnalyzer::getNextInputToken()
{
  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();
}

void SyntaxAnalyzer::error()
{
  string tc;
  string tokcodes[] =
      {
          "PLUS", "MINUS", "TIMES", "SLASH", "EQL", "LPAREN", "RPAREN", "PERIOD",
          "COMMA", "SEMICOLON", "COLON", "BECOMES", "LSS", "LEQ", "NEQ", "GTR",
          "GEQ", "ANDSYM", "BEGINSYM", "BOOLSYM", "DIVSYM", "DOSYM", "DOWNTOSYM",
          "ELSESYM", "ENDSYM", "FALSESYM", "FORSYM", "IFSYM", "INTSYM", "MODSYM",
          "NOTSYM", "ORSYM", "PROGSYM", "READSYM", "READLNSYM", "REALSYM",
          "REPEATSYM", "THENSYM", "TOSYM", "TRUESYM", "UNTILSYM", "VARSYM",
          "WHILESYM", "WRITESYM", "WRITELNSYM", "IDENT", "NUMLIT", "EOI", "NAL"};
  int errorIndex = la->getCurrentTokenPosition();
  for (int i = 0; i < errorIndex; i++)
  {
    cout << " ";
  }
  cout << "^\n";
  cout << "Error: ";
  cout << tokcodes[token_errors.back()];
  token_errors.pop_back();

  while (!token_errors.empty())
  {
    cout << " or ";
    cout << tokcodes[token_errors.back()];
    token_errors.pop_back();
  }
  cout << " expected.";
  exit(0);
}