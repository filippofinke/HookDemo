#include <iostream>

static void Print() {
    std::cout << "You called the Print function!" << std::endl;
}

int main()
{
    while (true) {
        Print();
        getchar();
    }
    return 0;
}
