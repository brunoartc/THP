#include <iostream> 
#include <regex> 
#include <string.h> 
using namespace std; 

class Token {
    public:
        string type;
        int value;
        Token(int _value){
            value = _value;
            type = extract_type(_value);
        }
        string extract_type(int value){
            if (isdigit(value)) return "DIGIT";
            else if (value == '-' | value == '+') return "OPERATOR";
            else return "NULL";
        }
        string get_type(){
            return type;
        }
};

class Tokenizer {
    public:
        string origin;
        int position = 0;
        Token* actual;
        Tokenizer(string _origin){
            origin = _origin;
            actual = new Token(origin.at(position));
        }
        
        void select_next(){
            if (position < origin.length()){
                position++;
                if(position == origin.length()){
                    actual->value = -1;
                    actual->type = "EOL";
                } else {
                    actual->value = origin.at(position);
                    actual->type = actual->extract_type(actual->value);
                }
                
                
                
            } else {
                //EOF
            }
            
        }
        string get_type(){
            return actual->type;
        }
        int get_value(){
            return actual->value;
        }
        
};

class Parser {
    public:
        Tokenizer* tokens;
        int parse_expression(){
            /**
             * PARSING TOKENS TO STACK 
             **/

            stack<int> s_digits;
            stack<char> s_operators;

            while (tokens->position < tokens->origin.length()) //TODO fix
            {
                if (tokens->get_type() == "DIGIT"){
                    //cout << "DIGIT";
                    //cout << "\n";
                    s_digits.push(tokens->get_value() - '0');
                } else if (tokens->get_type() == "OPERATOR"){
                    s_operators.push(tokens->get_value());
                }
                //cout << (char) tokens->get_value();
                //cout << "\n";
                tokens->select_next();
                
                
            }

            /**
             * EVALUATING STACK 
             **/

            while (s_operators.size() > 0)
            {   
                int y = s_digits.top(); s_digits.pop();
                int x = s_digits.top(); s_digits.pop();

                //cout << x;

                switch (s_operators.top())
                {
                
                case '+':
                    s_operators.pop();
                    s_digits.push(x+y);
                    break;
                case '-':
                    s_operators.pop();
                    s_digits.push(x-y);
                    break;
                
                default:
                    break;
                }
                
            }

            return s_digits.top();
            
            
        }

        int run(string code){
            tokens = new Tokenizer(code);
            return parse_expression();
            
        }
    
};

#ifndef _TESTS
int main(int argc, char const *argv[])
{
    Parser* parser = new Parser();

    cout << parser->run(argv[1]) << endl;
    return 0;
}
#endif

