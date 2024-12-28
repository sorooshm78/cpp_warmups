#include <iostream>

class Counter {
private:
    int count;

public:
    Counter(int start = 0) : count(start) {}

    int getCount() const {
        return count;
    }

    void increment() {
        ++count;
    }
};

int main() {
    Counter c1(10);
    std::cout << "Initial count: " << c1.getCount() << std::endl; // OK
    c1.increment();
    std::cout << "After increment: " << c1.getCount() << std::endl; // OK

    const Counter c2(100);
    std::cout << "Const counter: " << c2.getCount() << std::endl; // OK
    //  c2.increment();  // ERROR: cannot call non-const function on a const object

    return 0;
}
