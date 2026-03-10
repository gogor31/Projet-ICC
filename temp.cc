#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

int main(){

    ifstream file("tests/test00.txt"); //open file

    if (!file.is_open()) {
        cout << "Error opening file\n" << endl;
        return 1;
    }

    string line;
    string content;

    while (getline(file, line)) {
        //cout << line << endl;
        content += line + "\n";
    }

    file.close();

    cout << content << endl;

    return 0;
}