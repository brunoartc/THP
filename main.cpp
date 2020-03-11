
#include <iostream> 
#include <regex> 
#include <string.h> 
using namespace std; 

string to_postfix(const string s1_raw){

    //PRE-PROCESSING
    regex comment("/\\*.*?\\*/");
    regex space(" ");

    const string s1 = regex_replace(regex_replace(s1_raw,comment, ""), space, "");

    regex infix(R"(([0-9]+)([+\-*/][0-9]+)*)");

    smatch sm;
    if (regex_match(s1, sm, infix) != true) {
        //cout << "Not";
        //throw 666;
    }
    string output;
    stack<int> operator_stack;

    map<char, int> precedence;

    precedence.insert({'-', 1});
    precedence.insert({'+', 1});
    precedence.insert({'/', 2});
    precedence.insert({'*', 2});
    precedence.insert({'(', 3});
    precedence.insert({')', 3});

    for (int i = 0; i < s1.size(); i++){
        if (isdigit(s1[i])){
            int num = 0;
            while(i<s1.length() && isdigit(s1[i])) {
				num = (num*10) + (s1[i] - '0'); 
				i++;
			}
            i--;
            output += to_string(num)+ ' ';
        } else if ((s1[i] == '-' | s1[i] == '+' | s1[i] == '*' | s1[i] == '/')){
            while (operator_stack.size() != 0 && precedence.at(s1[i]) <= precedence.at(operator_stack.top()) && operator_stack.top() != '('){
                output += operator_stack.top();
                operator_stack.pop();
            };
            
            operator_stack.push(s1[i]);
        } else if (s1[i] == '(') {
            operator_stack.push(s1[i]);
        } else if (s1[i] == ')') {
            while (operator_stack.top() != '(') {
                output += operator_stack.top();
                operator_stack.pop();

            }
            operator_stack.pop();
        }
    } 

    while (operator_stack.size() != 0){
        output += operator_stack.top();
        operator_stack.pop();
    }

    cout << output << "\n";
    return output;
}

int eval_postfix(string postfix_exp){
	stack<int> postfix_stack;


    regex operators(R"([+\-*/])");
    
	for(int i = 0;i< postfix_exp.size();i++) {

        smatch sm;

        int operand2, operand1;
        switch (postfix_exp[i])
        {
            
            
        case '-':        
            operand2 = postfix_stack.top(); postfix_stack.pop();
			operand1 = postfix_stack.top(); postfix_stack.pop();
            postfix_stack.push(operand1 - operand2);
            break;

        case '+': 
               
            operand2 = postfix_stack.top(); postfix_stack.pop();
			operand1 = postfix_stack.top(); postfix_stack.pop();
            postfix_stack.push(operand1 + operand2);
            break;
        case '*': 
               
            operand2 = postfix_stack.top(); postfix_stack.pop();
			operand1 = postfix_stack.top(); postfix_stack.pop();
            postfix_stack.push(operand1 * operand2);
            break;

        case '/': 
               
            operand2 = postfix_stack.top(); postfix_stack.pop();
			operand1 = postfix_stack.top(); postfix_stack.pop();
            postfix_stack.push(operand1 / operand2);
            break;


        default:
            int end_num = postfix_exp.substr(i).find(' ') + i;
            postfix_stack.push(stoi(postfix_exp.substr(i, end_num)));

            i += end_num - i;
            
            break;
        }
	}
    
	return postfix_stack.top();
}

#ifndef _TESTS
int main(int argc, char const *argv[]){
    cout << eval_postfix(to_postfix(argv[1]));
    cout << "\n";
    return 0;
}
#endif