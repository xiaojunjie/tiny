#include <iostream>
#include <vector>
using namespace std;

typedef struct{
    int a;
} A;
int main(){
    vector<string> *vecptr = new vector<string>(1);
    vecptr->push_back("1");
    vecptr->push_back("2");
    vecptr->push_back("3");
    cout << (*vecptr)[0] << endl;
    cout << (*vecptr)[1] << endl;
    cout << (*vecptr)[2] << endl;
    return 0;
}
