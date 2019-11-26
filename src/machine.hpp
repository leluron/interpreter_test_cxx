#pragma once

#include <string>
#include <map>
#include <cstdlib>
#include <deque>

#include "lexer.hpp"
#include "ast.hpp"

class Machine {
public:
    void interpret(std::string expr);
    std::string getOutput() { return output;}
    void clearOutput() { output = ""; }
private:
    void execStatement(std::shared_ptr<Statement> s);
    double eval(std::shared_ptr<Expr> e);
    std::string output;
};