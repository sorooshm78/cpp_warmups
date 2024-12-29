#include <iostream>

class Fraction {
public:
    int numerator;
    int denominator;

    // Single-argument constructor
    Fraction(int num, int denom = 1) : numerator(num), denominator(denom) {}

    void display() const {
        std::cout << numerator << "/" << denominator << std::endl;
    }
};

void printFraction(const Fraction& frac) {
    frac.display();
}

int main() {
    Fraction f1 = 5; // Implicit conversion from int to Fraction
    f1.display();    // Outputs: 5/1

    printFraction(10); // Implicitly converts 10 to Fraction(10, 1)
    return 0;
}
