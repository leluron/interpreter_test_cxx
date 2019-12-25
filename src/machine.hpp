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
    double getVariable(std::string var);
private:
    void execStatement(std::shared_ptr<Statement> s);
    void execAssign(std::shared_ptr<AssignStatement> s);
    double eval(std::shared_ptr<Expr> e);

    table vars;
};