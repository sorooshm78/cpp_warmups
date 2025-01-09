#include<string>


class TestHelper
{
public:
    template<typename T>
    static T getValue();
};


template<>
int TestHelper::getValue<int>()
{
    return 10;
}

template<>
std::string TestHelper::getValue<std::string>()
{
    return "hello";
}
