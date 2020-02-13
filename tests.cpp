#define _TESTS
#include "main.cpp"


int main(int argc, char const *argv[])
{
    if (reverse_polish("1+1 +1 +1 +1").size() != 1) return -1;
    if (reverse_polish("1+1 +1 +1 +1").top() != 5) return -1;
    if (reverse_polish("1+1 +1 +1 +1 - 1").top() != 4) return -1;
    if (reverse_polish("1+1 +1 +1 +1 1").size() != 2) return -1;
    if (reverse_polish("1+1 +1 +1 +1 + 1 - - - - ").size() != 1) return -1;
    cout << "Done\n";
    return 0;
}
