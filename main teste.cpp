#include <string.h>
#include <iostream>
#include <regex>
#include <math.h>
#include "nodes.cpp"
using namespace std;

#define UNKNOWNTOKEN 1;
#define UNEXPECTEDTOKEN 2;

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
      return "UNOP";
    else if (value == '/' | value == '*')
      return "BIOP";
    else if (value == '(' | value == ')')
      return "BRCKT";
    else if (value == ' ')
      return "SPACE";
    else
      throw UNKNOWNTOKEN;
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
      // EOF or SPACE
    }
  }
  string get_type() { return actual->type; }
  int get_value() { return actual->value; }
};

class Parser {
 public:
  Tokenizer* tokens;
  Node parse_expression() {
    Node output = parse_term();

    while (tokens->get_type() == "UNOP") {
      output = BinOp();
      output.value = tokens->get_value();
      output.children.push_back(output);
      output.children.push_back(parse_term());
    }

    return output;
  }

  Node parse_term() {
    Node output = parse_factor();

    while (tokens->get_type() == "BIOP") {
      output = BinOp();
      output.value = tokens->get_value();
      output.children.push_back(output);
      output.children.push_back(parse_term());
    }

    return output;
  }

  Node parse_factor() {
    int resp = 0;
    Node output;


    tokens->select_next();

    if (tokens->get_type() == "DIGIT"){ //while
      cout << "DIGIT";
      output = Num();
      output.value = tokens->get_value() - '0';
      tokens->select_next();
    } else if (tokens->get_type() == "BRCKT") {
      if (tokens->get_value() == '('){
        if (tokens->get_value() == ')'){
          tokens->select_next();
        } else {
          throw UNEXPECTEDTOKEN;
        }
      } else {
        throw UNEXPECTEDTOKEN;
      }
    } else if (tokens->get_type() == "UNOP"){
      output = UnOp();
      output.value = tokens->get_value();
      output.children.push_back(parse_term());
    } else {
      throw UNEXPECTEDTOKEN;
    }
    return output;

  }

  int run(string code) {
    tokens = new Tokenizer(code);
    Node parser = parse_expression();
    return parser.evaluate();
  }
};

#ifndef _TESTS
int main(int argc, char const* argv[]) {
  Parser* parser = new Parser();
  string code = argv[1];
  //reverse(code.begin(), code.end());

  cout << parser->run(code) << endl;
  return 0;
}
#endif
