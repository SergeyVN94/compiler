#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <string>

using namespace std;

// Токены
enum TOKENS {
  ID = 256,
  NUM,
  TRUE,
  FALSE,
  EQUALLY,
  LESS_OR_EQUAL,
  MORE_OR_EQUAL,
  NOT_EQUAL,
};

struct Token {
  uint name;
  long double value;
  std::string lexeme;
};

#endif
