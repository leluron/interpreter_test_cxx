#include <iostream>
#include <fstream>
#include <string>

#include "machine.hpp"

using namespace std;

int main(int argc, char **argv) {

    string text;

    {
        ifstream file("script.txt");
        file.seekg(0, ios::end);
        size_t len = file.tellg();
        text.resize(len);
        file.seekg(0, ios::beg);
        file.read(text.data(), len);
    } 

    Machine machine;

    machine.interpret(text);

    cout << machine.getVariable("var3") << endl;
}