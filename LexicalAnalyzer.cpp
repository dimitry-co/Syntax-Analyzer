#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <map>

#include "LexicalAnalyzer.h"
#include "Token.h"
#include "TokenCodes.h"

using namespace std;

LexicalAnalyzer::LexicalAnalyzer(ifstream *scf)
{
  sourceCodeFile = scf;
  for (int i = 0; i < 49; i++)
  {
    tokenMap[lexList[i]] = i;
  }
  currentIndOfLine = 0;
}

string LexicalAnalyzer::lexList[49] = {"+", "-", "*", "/", "=", "(", ")", ".", ",", ";",
                                       ":", ":=", "<", "<=", "<>", ">", ">=", "and", "begin",
                                       "boolean", "div", "do", "downto", "else", "end", "false",
                                       "for", "if", "integer", "mod", "not", "or", "program",
                                       "read", "readln", "real", "repeat", "then", "to", "true",
                                       "until", "var", "while", "write", "writeln", "identifier",
                                       "numeric literal", "end of input", "not a lexeme"};

Token *LexicalAnalyzer::getNextToken()
{

  getTokenPosAndCharClass();

  Token *t;
  lexeme = new string;

  switch (charClass)
  {
  case 1:
    findLettOrResWord();
    break;
  case 2:
    getNum();
    break;
  case 3:
    check_Valid_Single_or_Double_Chars();
    break;
  case 4:
    *lexeme = "end of input";
    newtc = (TokenCodes)(tokenMap.at(*lexeme));
    break;
  }
  t = new Token(newtc, *lexeme);
  delete lexeme;
  return t;
}

void LexicalAnalyzer::getTokenPosAndCharClass()
{
  if (currentIndOfLine == 0 && !sourceCodeFile->eof())
  {
    getline(*sourceCodeFile, line);
    end_of_line = line.length();
    getFirstChar();
    getCharClass();
  }
  else if (currentIndOfLine == end_of_line || (currentIndOfLine + 1 == end_of_line && line[currentIndOfLine] == ' '))
  {
    sourceCodeFile->unget();
    if (sourceCodeFile->get() == '\n')
    {
      getline(*sourceCodeFile, line);
      end_of_line = line.length();
      cout << line << endl;
      currentIndOfLine = 0;
      getFirstChar();
      getCharClass();
    }
    else if (sourceCodeFile->peek() == -1)
    {
      if (sourceCodeFile->eof())
      {
        charClass = 4;
      }
    }
  }
  else
  {
    getFirstChar();
    getCharClass();
  }
}

void LexicalAnalyzer::getFirstChar()
{
  int i = currentIndOfLine;
  if (i + 1 == end_of_line)
  {
    nextChar = line[i];
    currentTokenPosition = i;
    return;
  }
  while (i + 1 <= end_of_line)
  {
    if (line[i] != ' ')
    {
      if (i == 0)
      {
        currentTokenPosition = i;
        currentIndOfLine = i;
        nextChar = line[i];
        break;
      }
      else
      {
        currentTokenPosition = i;
        currentIndOfLine = i;
        nextChar = line[i];
        break;
      }
    }
    i++;
  }
}

void LexicalAnalyzer::getCharClass()
{
  if (isalpha(nextChar))
  {
    charClass = 1;
  }
  else if (isdigit(nextChar))
  {
    charClass = 2;
  }
  else if (currentIndOfLine < line.length())
  {
    charClass = 3;
  }
  else
  {
    charClass = 4;
  }
}

void LexicalAnalyzer::findLettOrResWord()
{
  getNextChar();
  while (isalnum(nextChar) && currentIndOfLine + 1 <= line.length())
  {
    getNextChar();
  }
  if (std::any_of(lexeme->begin(), lexeme->end(), ::isdigit))
  {
    newtc = (TokenCodes)(tokenMap.at("identifier"));
  }
  else if (tokenMap.find(*lexeme) == tokenMap.end())
  {
    newtc = (TokenCodes)(tokenMap.at("identifier"));
  }
  else
  {
    newtc = (TokenCodes)(tokenMap.at(*lexeme));
  }
}

void LexicalAnalyzer::getNum()
{
  getNextChar();
  while (isdigit(nextChar) && (currentIndOfLine + 1 <= line.length()))
  {
    getNextChar();
  }
  if (nextChar == '.' && (currentIndOfLine + 1 <= line.length()))
  {
    getNextChar();
    if (isdigit(nextChar))
    {
      while (isdigit(nextChar) && (currentIndOfLine + 1 <= line.length()))
      {
        getNextChar();
      }
    }
  }
  newtc = (TokenCodes)(tokenMap.at("numeric literal"));
}

void LexicalAnalyzer::check_Valid_Single_or_Double_Chars()
{
  getNextChar();
  if (tokenMap.find(*lexeme) == tokenMap.end())
  {
    *lexeme = "not a lexeme";
  }
  else if (currentIndOfLine + 1 <= end_of_line && nextChar != ' ')
  {
    getNextChar();
    if (tokenMap.find(*lexeme) == tokenMap.end())
    {
      lexeme->pop_back();
      currentIndOfLine--;
    }
  }

  newtc = (TokenCodes)(tokenMap.at(*lexeme));
}

void LexicalAnalyzer::getNextChar()
{
  lexeme->push_back(nextChar);
  if (currentIndOfLine + 1 <= line.length())
  {
    currentIndOfLine++;
    nextChar = line[currentIndOfLine];
  }
}

int LexicalAnalyzer::getCurrentTokenPosition()
{
  return currentTokenPosition;
}