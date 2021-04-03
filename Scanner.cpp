#ifndef __SCANNER_CPP__
#define __SCANNER_CPP__

#include <map>
#include <string>
#include "token.hpp"

using namespace std;

class Scanner {
 private:
  char *_buffer;
  uint _bufferSize;
  uint _bufferIndex;
  char _currentSymbol;
  bool _isEndBuffer;
  uint _line;
  std::map<string, Token> _words;

 private:
  bool _isDigit(char symbol);
  bool _isLetter(char symbol);
  void _nextSymbol();
  void _prevSymbol();
  void _skipUnnecessaryCharacters();
  void _reserveWord(uint id, string &s);
  double _parseFractionalPart();

 public:
  Scanner(char *buffer, uint bufferSize);
  ~Scanner();
  Token getNextToken();
  bool isEndScan();
  uint getCurrentLine();
};

Scanner::Scanner(char *buffer, uint bufferSize)
    : _buffer(buffer), _bufferSize(bufferSize) {
  _bufferIndex = 0;
  _currentSymbol = _buffer[0];
  _isEndBuffer = false;
  _line = 1;

  string t("true");
  string f("false");

  _reserveWord(TOKENS::TRUE, t);
  _reserveWord(TOKENS::FALSE, f);
}

Scanner::~Scanner() { delete _buffer; }

Token Scanner::getNextToken() {
  _skipUnnecessaryCharacters();

  Token token;

  if (_isDigit(_currentSymbol)) {
    uint value = 0;

    do {
      value = (value * 10) + (_currentSymbol - 48);
      _nextSymbol();
    } while (_isDigit(_currentSymbol));

    token.value = value;

    if (_currentSymbol == '.') {
      _nextSymbol();
      if (_isDigit(_currentSymbol)) token.value += _parseFractionalPart();
    }

    token.name = TOKENS::NUM;
    return token;
  }

  if (_isLetter(_currentSymbol)) {
    token.name = TOKENS::ID;
    token.lexeme = "";

    do {
      token.lexeme.push_back(_currentSymbol);
      _nextSymbol();
    } while (_isLetter(_currentSymbol) || _isDigit(_currentSymbol));
    
    if (_words.count(token.lexeme) != 0) {
      return _words.at(token.lexeme);
    }

    _words.insert(pair<string, Token>(token.lexeme, token));
    return token;
  }

  if (_currentSymbol == '.') {
    _nextSymbol();

    if (_isDigit(_currentSymbol)) {
      token.name = TOKENS::NUM;
      token.value = _parseFractionalPart();
      return token;
    } else {
      _prevSymbol();
    }
  }

  if (_currentSymbol == '=') {
    _nextSymbol();

    if (_currentSymbol == '=') {
      token.name = TOKENS::EQUALLY;
      token.lexeme = "==";
      return token;
    } else {
      _prevSymbol();
    }
  }

  if (_currentSymbol == '<') {
    _nextSymbol();

    if (_currentSymbol == '=') {
      token.name = TOKENS::LESS_OR_EQUAL;
      token.lexeme = "<=";
      return token;
    } else {
      _prevSymbol();
    }
  }

  if (_currentSymbol == '>') {
    _nextSymbol();

    if (_currentSymbol == '=') {
      token.name = TOKENS::MORE_OR_EQUAL;
      token.lexeme = ">=";
      return token;
    } else {
      _prevSymbol();
    }
  }

  if (_currentSymbol == '!') {
    _nextSymbol();

    if (_currentSymbol == '=') {
      token.name = TOKENS::NOT_EQUAL;
      token.lexeme = "!=";
      return token;
    } else {
      _prevSymbol();
    }
  }

  token.name = _currentSymbol;
  token.lexeme = _currentSymbol;
  _nextSymbol();

  return token;
}

bool Scanner::isEndScan() {
  return _isEndBuffer;
}

uint Scanner::getCurrentLine() {
  return _line;
}

bool Scanner::_isLetter(char symbol) {
  return (symbol > 64 && symbol < 91) || (symbol > 96 && symbol < 123) ||
         (symbol == 95);
}

bool Scanner::_isDigit(char symbol) { return (symbol > 47 && symbol < 58); }

void Scanner::_reserveWord(uint id, string &s) {
  Token token;
  token.name = id;
  token.lexeme = s;
  _words.insert(pair<string, Token>(token.lexeme, token));
}

void Scanner::_nextSymbol() {
  if (_bufferIndex < _bufferSize - 1) {
    _bufferIndex += 1;
    _currentSymbol = _buffer[_bufferIndex];
  } else {
    _currentSymbol = '\0';
    _isEndBuffer = true;
  }
}

void Scanner::_prevSymbol() {
  if (_bufferIndex > 0) _bufferIndex -= 1;
  _currentSymbol = _buffer[_bufferIndex];
}

void Scanner::_skipUnnecessaryCharacters() {
  while (true) {
    while (_currentSymbol == ' ' || _currentSymbol == '\t') _nextSymbol();

    if (_currentSymbol == '\n') {
      _line += 1;
      _nextSymbol();
      continue;
    }
    
    if (_currentSymbol == '/') {
      _nextSymbol();

      if (_currentSymbol == '/') {
        while (_currentSymbol != '\n') _nextSymbol();
        _line += 1;
        _nextSymbol();
        continue;
      } 
      
      if (_currentSymbol == '*') {
        _nextSymbol();

        while (true) {
          while (_currentSymbol != '*') {
            _nextSymbol();
            if (_currentSymbol == '\n') _line += 1;
          }

          _nextSymbol();
          if (_currentSymbol == '/') break;
        }
        
        _nextSymbol();
        continue;
      } 
        
      _prevSymbol();
      break;
    } 
    
    break;
  }
}

double Scanner::_parseFractionalPart() {
  double value = 0;
  uint fac = 10;

  while (_isDigit(_currentSymbol)) {
    value += (double)(_currentSymbol - 48) / (double)fac;
    fac = fac * 10;
    _nextSymbol();
  }

  return value;
} 

#endif
