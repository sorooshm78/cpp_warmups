#include <iostream>


using namespace std;


class ExampleMutable {
public:
    ExampleMutable(int val) : value(val), cache(0) {}

    // A const member function
    int getValue() const {
        // If cache is not mutable this line is compiler error
        // Even though the function is const, we can modify 'cache' because it's mutable
        cache = value * 2;
        return value;
    }

    // Another const member function
    int getCache() const {
        return cache;
    }

private:
    int value;
    mutable int cache; // 'cache' can be modified in const functions
};


int main() {
    const ExampleMutable ex(10);
    
    std::cout << "Value: " << ex.getValue() << std::endl; // Modifies 'cache'
    std::cout << "Cache: " << ex.getCache() << std::endl; // Accesses 'cache'

    return 0;
}