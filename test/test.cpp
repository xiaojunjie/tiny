#include <vector>
#include <iostream>
#include <functional>
using namespace std;
typedef function<void(int)> make_response_function;

void test(make_response_function func) {
    func(1);
}
int main(int argc, char const *argv[]) {
    int b;
    test([b](int a){
        std::cout << a << std::endl;
        std::cout << b << std::endl;
    });
    return 0;
}
