#include <iostream>

class MyClass {
public:
    int value;

    MyClass(int v) : value(v) {}

    void display() const {
        std::cout << "Value: " << value << std::endl;
    }
};

int main() {
    const MyClass obj(5);
    obj.display();
//     obj.value = 10;
    return 0;
}
