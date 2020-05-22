#include <math.h>
#include <string.h>

#include <iostream>
#include <regex>

#include <fstream>

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
    else if (value == '$')
      return "VAR";
    else if (value == '{' | value == '}')
      return "CBRCKT";
    else if (value == '=')
      return "EQUALS";
    else if (value == ';')
      return "END";
    else if (isalpha(value))
      return "ALPHA";
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
    #ifdef DEBUG
    cout << position << "              <-POSITION" << endl;
    #endif
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

      while ( position + 1 < origin.length() && ((actual->type == "DIGIT" && actual->extract_type(origin.at(position + 1)) == "DIGIT")  | (actual->type == "VAR" && actual->extract_type(origin.at(position + 1)) == "ALPHA")  | (actual->type == "ALPHA" && actual->extract_type(origin.at(position + 1)) == "ALPHA")))
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
  string get_full_value(){
    return actual->value;
  }
};

class Parser
{
public:
  Tokenizer *tokens;

  
  static Node parse_block(Tokenizer *tokens)
  {
    vector<Node> blocks;
    while (tokens->get_type() == "CBRCKT")
    {
      if (tokens->get_value() == '{'){
        //blocks.push_back(parse_command()) TODO !!!
        tokens->select_next();
        if (tokens->get_value() == '}'){
          tokens->select_next();
        } else {
        throw UNEXPECTEDTOKEN;
        }
      } else {
        throw UNEXPECTEDTOKEN;
      }
    }
    return Block(blocks);
  }


  static Node parse_command2(Tokenizer *tokens)
  {
    Node output = parse_term(tokens);
    vector<Node> blocks;
    while (tokens->get_type() == "CBRCKT")
    {
      if (tokens->get_value() == '{'){
        //blocks.push_back(parse_command()) TODO !!!
        tokens->select_next();
        if (tokens->get_value() == '}'){
          tokens->select_next();
        } else {
        throw UNEXPECTEDTOKEN;
        }
      } else {
        throw UNEXPECTEDTOKEN;
      }
    }
    output = Block(blocks);
    return output;
  }



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
    } else if (tokens->get_type() == "VAR"){

    }
    tokens->select_next();
    return output;
  }

  static Node parse_command(Tokenizer *tokens)
  {
    tokens->select_next();
    Node output;

    if (tokens->get_type() == "END"){
      output = NoOp();
    } else if(tokens->get_type() == "VAR") {

      Node var_node = Var(tokens->get_full_value());
      tokens->select_next();


      if (tokens->get_type() == "EQUALS") {
        tokens->select_next();
        
        vector<Node> partial_output;
        partial_output.push_back(var_node);
        partial_output.push_back(parse_expression(tokens));
        string token_value = tokens->get_full_value();
        
        tokens->select_next();

        if(tokens->get_type() == "END") {
          return Equal(token_value, partial_output);
        } else {
          throw UNKNOWNTOKEN;
        }

      } else {
        throw UNKNOWNTOKEN;
      }

    } else if (tokens->get_type() == "ALPHA") {

      if (tokens->get_full_value() == "ECHO") {
        tokens->select_next();
        vector<Node> partial_output;
        partial_output.push_back(parse_expression(tokens));
        output = Echo(partial_output);
        tokens->select_next(); // o proximo chamado nao pode dar select next

        if(tokens->get_type() == "END") {
          return output;
        } else {
          throw UNKNOWNTOKEN;
        }

      } else if (tokens->get_full_value() == "WHILE"){
        tokens->select_next();
        if (tokens->get_value() == '('){
          tokens->select_next();
          //Node real_exp = parse_rel_expre() TODO
          tokens->select_next();
          if ((tokens->get_value() == '(')) {
            tokens->select_next();
            //Node command = parse_command() TODO
            tokens->select_next();
            //return While(partial_output) TODO
          } else {
            throw UNEXPECTEDTOKEN;
          }
        } else {
          throw UNEXPECTEDTOKEN;
        }
        
      } else if (tokens->get_full_value() == "IF"){
        vector<Node> partial_output;
        tokens->select_next();
        if (tokens->get_value() == '('){
          tokens->select_next();
          //Node real_exp = parse_rel_expre() TODO
          tokens->select_next();
          if ((tokens->get_value() == '(')) {
            tokens->select_next();
            partial_output.push_back(parse_command(tokens));// TODO
            tokens->select_next();
            if (tokens->get_full_value() == "ELSE"){
              tokens->select_next();
              partial_output.push_back(parse_command(tokens));
              tokens->select_next();
            }
            //return If(partial_output) TODO
          } else {
            throw UNEXPECTEDTOKEN;
          }
        } else {
          throw UNEXPECTEDTOKEN;
        }
        
      }

    } else {
      return parse_block(tokens);
      
    }
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
    return s1;
}

#ifndef _TESTS
int main(int argc, char const *argv[])
{
  Parser *parser = new Parser();
  ifstream ifs(argv[1]);
  string code( (istreambuf_iterator<char>(ifs) ),
                       (istreambuf_iterator<char>()    ) );
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
