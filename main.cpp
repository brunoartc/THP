#include <iostream> 
#include <regex> 
#include <string.h> 
using namespace std; 








stack<int> reverse_polish(string s1){
    regex numbers(R"([0-9]+)");
    regex operators(R"([+\-*/%])");


    sregex_iterator iter_numbers(s1.begin(), s1.end(), numbers);
    string s2 = s1;
    reverse(s2.begin(), s2.end()); 

    sregex_iterator iter_operators(s2.begin(), s2.end(), operators);
    sregex_iterator end;

    stack <int> number_stack;


    while(iter_numbers != end)
    {

        for(unsigned i = 0; i < iter_numbers->size(); ++i)
        {
            number_stack.push(stoi((*iter_numbers)[i]));

        }
        ++iter_numbers;
    }




    while(iter_operators != end)
    {
        int x, y;
        for(unsigned i = 0; i < iter_operators->size(); ++i)
        {
            
            string result_string = (*iter_operators)[i];
            if (number_stack.size() < 2){
                cout << "WARN:\tInput Error\n";
                break;
            } 
            switch (result_string[0])
            {
            case '+':
                
                y = number_stack.top();
                number_stack.pop();
                x = number_stack.top();
                number_stack.pop();
                cout << x;
                cout << '+';
                cout << y;
                cout << "\n";


                number_stack.push(x+y);
                break;
            case '-':
                y = number_stack.top();
                number_stack.pop();
                x = number_stack.top();
                number_stack.pop();

                cout << x;
                cout << '-';
                cout << y;
                cout << "\n";

                number_stack.push(x-y);
                break;
            
            default:
                break;
            }

        }
        ++iter_operators;
    }

    if (number_stack.size() != 1){
        cout << "WARN:\tInput Error\n";
    }
    
    return number_stack;
}

#ifndef _TESTS
int main(int argc, char const *argv[]){
    cout << reverse_polish(argv[1]).top();
    cout << "\n"
    return 0;
}
#endif