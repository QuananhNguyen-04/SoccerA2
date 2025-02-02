#include <iostream>
// #include <SDL2/SDL.h>
using namespace std;

int main(){
    if (__cplusplus == 202002L) cout << "C++20";
    if (__cplusplus == 201703L) cout << "C++17";
    if (__cplusplus == 201402L) cout << "C++14";
    if (__cplusplus == 201103L) cout << "C++11";
    int *a = new int[10];
    a[0] = 1;
    for (int i = 0; i < 10; ++i) {
        a[i] = i;
    }
    delete a;
    cout << a[9] << endl;
    cout << __cplusplus << endl;
}