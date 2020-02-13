#include <iostream> 
#include <regex> 
#include <string.h> 
using namespace std; 








stack<int> reverse_polish(string s1){
    regex numbers(R"([0-9]+)");
    regex operators(R"([+\-*/%])");


    sregex_iterator iter_numbers(s1.begin(), s1.end(), numbers);

    sregex_iterator iter_operators(s1.begin(), s1.end(), operators);
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
                
                x = number_stack.top();
                number_stack.pop();
                y = number_stack.top();
                number_stack.pop();

                number_stack.push(x+y);
                break;
            case '-':
                x = number_stack.top();
                number_stack.pop();
                y = number_stack.top();
                number_stack.pop();

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
    return 0;
}
#endif