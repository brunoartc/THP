#include <math.h>
#include <string.h>

#include <iostream>
#include <regex>
#include <fstream>

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
    else if (value == ' ' | value == '\n')
      return "SPACE";
    else if (value == '{' | value == '}')
      return "BRCKTT";

    else if (value == '$')  // fix
      return "VAR";
    else if (value == '=')  // fix
      return "EQUALS";
    else if (value == ';')  // fix
      return "END";
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
      if (position == origin.length()) {
        actual->value = -1;
        actual->type = "EOL";
      } else {
        actual->value = origin.at(position);
        actual->type = actual->extract_type(actual->value);
      }

      position++;

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
  static Node parse_expression(Tokenizer* tokens) {
    Node output = parse_term(tokens);

    while (tokens->get_type() == "UNOP") {
      int token = tokens->get_value();
      vector<Node> partial_output;
      partial_output.push_back(output);
      partial_output.push_back(parse_term(tokens));
      output = BinOp(token, partial_output);
    }
    return output;
  }

  static Node parse_term(Tokenizer* tokens) {
    Node output = parse_fator(tokens);

    while (tokens->get_type() == "BIOP") {
      int token = tokens->get_value();
      vector<Node> partial_output;
      partial_output.push_back(output);
      partial_output.push_back(parse_term(tokens));
      output = BinOp(token, partial_output);
    }
    return output;
  }

  static Node parse_block(Tokenizer* tokens) {
    Node output;
    if (tokens->get_type() == "BRCKTT") {
      if (tokens->get_value() == '{') {
        while (tokens->get_value() != '}') {
          output = parse_command(tokens);
        }
      }
    }
  }

  static Node parse_command(Tokenizer* tokens) {
    Node output;

    if (tokens->get_type() == "VAR") {
      Node output1 = Var(tokens->get_value());
      tokens->select_next();
      if (tokens->get_type() == "EQUALS") {
        vector<Node> partial_output;
        partial_output.push_back(output1);
        tokens->select_next();
        partial_output.push_back(parse_expression(tokens));
        output = Equal('=', partial_output);
        tokens->select_next();
        if (tokens->get_type() != "END") {
          throw 666;
        }
      }
    } else if (tokens->get_type() == "END") {
      output = NoOp();
    } else if (tokens->get_type() == "ECHO") {
      tokens->select_next();
      vector<Node> partial_output;
      partial_output.push_back(parse_expression(tokens));
      output = Echo(477, partial_output);
      tokens->select_next();
      if (tokens->get_type() != "END") {
        throw 666;
      }

    } else {
      tokens->select_next();
      output = parse_block(tokens);
    }

    return output;
  }

  static Node parse_fator(Tokenizer* tokens) {  // mudar
    tokens->select_next();
    Node output;

    if (tokens->get_type() == "DIGIT") {
      output = Num(tokens->get_value() - '0');

#ifdef DEBUG
      cout << output.evaluate(&output);
#endif
    } else if (tokens->get_type() == "UNOP") {
      int token = tokens->get_value();
      vector<Node> partial_output;
      partial_output.push_back(parse_fator(tokens));
      output = UnOp(token, partial_output);
#ifdef DEBUG
      cout << token << partial_output[0].evaluate(&partial_output[0]);
#endif
    } else if (tokens->get_type() == "BRCKT") {
      if (tokens->get_value() == '(') {
        output = parse_expression(tokens);

        if (tokens->get_value() != ')') {
          throw UNEXPECTEDTOKEN;
        }
      }
    } else if (tokens->get_type() == "VAR") {
      output = Var(tokens->get_value());
    }
    tokens->select_next();
    return output;
  }

  static Node run(string code) {
    Node parser = parse_expression(new Tokenizer(code));
    return parser;
  }
};

#ifndef _TESTS
int main(int argc, char const* argv[]) {
  Parser* parser = new Parser();


  std::ifstream ifs(argv[1]);
  std::string content( (std::istreambuf_iterator<char>(ifs)),
                       (std::istreambuf_iterator<char>()));




  regex comment("/\\*.*?\\*/");
  regex space(" ");

  const string code = regex_replace(regex_replace(content, comment, ""), space, "");

  Node teste = parser->run(code);

  cout << teste.evaluate(&teste)
#ifdef DEBUG
       << "<-RESP"
#endif
       << endl;
  return 0;
}
#endif
