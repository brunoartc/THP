#include <math.h>
#include <string.h>

#include <iostream>
#include <regex>

#include "nodes.cpp"
using namespace std;

#define UNKNOWNTOKEN 1;
#define UNEXPECTEDTOKEN 2;

#define DEBUG

class Token
{
public:
  string type;
  string value;
  Token(int _value)
  {
    value = _value;
    type = extract_type(_value);
  }
  string extract_type(int value)
  {
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
    else if (value == '$' | isalpha(value))
      return "VAR";
    else if (value == '{' | value == '}')
      return "CBRCKT";
    else if (value == '=')
      return "EQUALS";
    else if (value == ';')
      return "END";
    else
      cout << (char) value;
    return "NULL";
  }
  string get_type() { return type; }
};

class Tokenizer
{
public:
  string origin;
  int position = 0;
  Token *actual;
  Tokenizer(string _origin)
  {
    origin = _origin;
    actual = new Token(origin.at(position));
  }

  void select_next()
  {
    cout << position << "              <-POSITION" << endl;
    if (position < origin.length())
    {
      if (position == origin.length())
      {
        actual->value = -1;
        actual->type = "EOL";

      }
      else
      {
        actual->value = origin.at(position);
        actual->type = actual->extract_type(actual->value.at(0));

      }

      while ((actual->type == "DIGIT" | actual->type == "VAR") && position + 1 < origin.length() && actual->type == actual->extract_type(origin.at(position + 1)))
      {
        position++;
        actual->value += origin.at(position);

      }
      #ifdef DEBUG
      cout << actual->value << "->" << actual->type << endl;
      #endif
      position++;
    }
    else
    {
      #ifdef DEBUG
      cout << actual->value << "!>" << actual->type << endl;
      #endif
      // EOF or SPACE
    }
  }
  string get_type() { return actual->type; }
  int get_value()
  {
    if (actual->type == "DIGIT")
    {

      return stoi(actual->value);
    }
    else if (actual->value.size() > 1)
    {
      return -1;
    }
    else
    {
      return actual->value.at(0);
    }
  }
};

class Parser
{
public:
  Tokenizer *tokens;
  static Node parse_expression(Tokenizer *tokens)
  {
    Node output = parse_term(tokens);

    while (tokens->get_type() == "UNOP")
    {
      int token = tokens->get_value();
      vector<Node> partial_output;
      partial_output.push_back(output);
      partial_output.push_back(parse_term(tokens));
      output = BinOp(token, partial_output);
    }
    return output;
  }

  static Node parse_term(Tokenizer *tokens)
  {
    Node output = parse_fator(tokens);

    while (tokens->get_type() == "BIOP")
    {
      int token = tokens->get_value();
      vector<Node> partial_output;
      partial_output.push_back(output);
      partial_output.push_back(parse_term(tokens));
      output = BinOp(token, partial_output);
    }
    return output;
  }

  static Node parse_fator(Tokenizer *tokens)
  {
    tokens->select_next();
    Node output;

    if (tokens->get_type() == "DIGIT")
    {
      output = Num(tokens->get_value());
    }
    else if (tokens->get_type() == "UNOP")
    {
      int token = tokens->get_value();
      vector<Node> partial_output;
      partial_output.push_back(parse_fator(tokens));
      output = UnOp(token, partial_output);
    }
    else if (tokens->get_type() == "BRCKT")
    {
      if (tokens->get_value() == '(')
      {
        output = parse_expression(tokens);

        if (tokens->get_value() != ')')
        {
          throw UNEXPECTEDTOKEN;
        }
      }
    }
    tokens->select_next();
    return output;
  }

  static Node run(string code)
  {
    Node parser = parse_expression(new Tokenizer(code));
    return parser;
  }
};


string pre_processing(string code) {
  regex comment("/\\*.*?\\*/");
    regex space(" ");

    const string s1 = regex_replace(code,comment, "");
    cout << s1;
    return s1;
}

#ifndef _TESTS
int main(int argc, char const *argv[])
{
  Parser *parser = new Parser();
  string code = argv[1];
  string pre_processed = pre_processing(code);
  Node teste = parser->run(pre_processed);
  cout << teste.evaluate(&teste)
#ifdef DEBUG
       << "<-RESP"
#endif
       << endl;
  return 0;
}
#endif
