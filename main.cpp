#include <string.h>
#include <iostream>
#include <regex>
using namespace std;

class Token {
 public:
  string type;
  int value;
  Token(int _value) {
    value = _value;
    type = extract_type(_value);
  }
  string extract_type(int value) {
    if (isdigit(value))
      return "DIGIT";
    else if (value == '-' | value == '+')
      return "OPERATOR";
    else
      return "NULL";
  }
  string get_type() { return type; }
};

class Tokenizer {
 public:
  string origin;
  int position = 0;
  Token* actual;
  Tokenizer(string _origin) {
    origin = _origin;
    actual = new Token(origin.at(position));
  }

  void select_next() {
    if (position < origin.length()) {
      position++;
      if (position == origin.length()) {
        actual->value = -1;
        actual->type = "EOL";
      } else {
        actual->value = origin.at(position);
        actual->type = actual->extract_type(actual->value);
      }
    } else {
      // EOF
    }
  }
  string get_type() { return actual->type; }
  int get_value() { return actual->value; }
};

class Parser {
 public:
  Tokenizer* tokens;
  int parse_expression() {
    /**
     * PARSING TOKENS TO STACK
     **/

    stack<int> s_digits;
    stack<char> s_operators;
    int digi_multi = 0;
    while (tokens->position < tokens->origin.length())  // TODO fix
    {
      if (tokens->get_type() == "DIGIT") {
        int tmp = 0;
        if (digi_multi) {
          tmp = s_digits.top()  + ((tokens->get_value() - '0') * 10 * digi_multi); s_digits.pop();
        } else {
          tmp = (tokens->get_value() - '0');
        }
        digi_multi += 1;
        s_digits.push(tmp);
      } else if (tokens->get_type() == "OPERATOR") {
        digi_multi = 0;
        s_operators.push(tokens->get_value());
      }
      tokens->select_next();
    }

    /**
     * EVALUATING STACK
     **/

    while (s_operators.size() > 0) {
      int x = s_digits.top();
      s_digits.pop();
      int y = s_digits.top();
      s_digits.pop();

      switch (s_operators.top()) {
        case '+':
          s_operators.pop();
          s_digits.push(x + y);
          break;
        case '-':
          s_operators.pop();
          s_digits.push(x - y);
          break;

        default:
          break;
      }
    }

    return s_digits.top();
  }

  int run(string code) {
    tokens = new Tokenizer(code);
    return parse_expression();
  }
};

#ifndef _TESTS
int main(int argc, char const* argv[]) {
  Parser* parser = new Parser();
  string code = argv[1];
  reverse(code.begin(), code.end());

  cout << parser->run(code) << endl;
  return 0;
}
#endif
