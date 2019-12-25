#include "machine.hpp"
#include "lexer.hpp"
#include "ast.hpp"
#include "astbuilder.hpp"

#include <iostream>

using namespace std;

void Machine::interpret(string expr) {
    auto tokens = lexer(expr.c_str());
    auto st = ASTBuilder(tokens).parseStatementList();
    for (auto s : st) {
        s->exec(vars);
    }
}

void AssignStatement::exec(table &t) {
    t[id] = e->eval(t);
}

double NumberExpr::eval(const table &t) {
    return value;
}

double BOpExpr::eval(const table &t) {
    auto l = e1->eval(t);
    auto r = e2->eval(t);
    switch (type) {
        case BOpType::Plus : return l+r;
        case BOpType::Minus: return l-r;
        case BOpType::Mul  : return l*r;
        case BOpType::Div  : return l/r;
        default: throw runtime_error("Not Supported");
    }
}

double IdentifierExpr::eval(const table &t) {
    auto it = t.find(name);
    if (it == t.end()) throw runtime_error("Variable doesn't exist");
    return it->second;
}

double Machine::getVariable(std::string var) {
    return vars[var];
}