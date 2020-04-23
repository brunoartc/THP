#include <math.h>
#include <string.h>

#include <iostream>
#include <regex>

#include "nodes.cpp"
using namespace std;

#define UNKNOWNTOKEN 1;
#define UNEXPECTEDTOKEN 2;

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
    else
      throw UNKNOWNTOKEN;
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

      while (position + 1 < origin.length() && actual->type == actual->extract_type(origin.at(position + 1)))
      {
        position++;
        actual->value += origin.at(position);
      }

      position++;
    }
    else
    {
      // EOF or SPACE
    }
  }
  string get_type() { return actual->type; }
  int get_value()
  {
    if (actual->value.size() > 1)
    {
      if (actual->type == "DIGIT")
      {

        return stoi(actual->value);
      }
    } else {
      if (actual->type == "DIGIT")
      {

        return stoi(actual->value);
      } else {
        return actual->value.at(0);
      }
      
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

#ifdef DEBUG
      cout << output.evaluate(&output);
#endif
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

#ifndef _TESTS
int main(int argc, char const *argv[])
{
  Parser *parser = new Parser();
  string code = argv[1];
  Node teste = parser->run(code);

  cout << teste.evaluate(&teste)
#ifdef DEBUG
       << "<-RESP"
#endif
       << endl;
  return 0;
}
#endif
