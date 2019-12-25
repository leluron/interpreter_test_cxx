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

shared_ptr<Value> NumberExpr::eval(const table &t) {
    return shared_ptr<Value>(new ValueNumber(value));
}

shared_ptr<Value> BOpExpr::eval(const table &t) {
    auto l = e1->eval(t);
    auto r = e2->eval(t);
    if (l->kind() != ValueKind::Number ||
        r->kind() != ValueKind::Number) throw runtime_error("Should be numbers for BOp");
    double lv = dynamic_pointer_cast<ValueNumber>(l)->value();
    double rv = dynamic_pointer_cast<ValueNumber>(r)->value();
    switch (type) {
        case BOpType::Plus : return shared_ptr<Value>(new ValueNumber(lv+rv));
        case BOpType::Minus: return shared_ptr<Value>(new ValueNumber(lv-rv));
        case BOpType::Mul  : return shared_ptr<Value>(new ValueNumber(lv*rv));
        case BOpType::Div  : return shared_ptr<Value>(new ValueNumber(lv/rv));
        default: throw runtime_error("Not Supported");
    }
}

shared_ptr<Value> IdentifierExpr::eval(const table &t) {
    auto it = t.find(name);
    if (it == t.end()) throw runtime_error("Variable doesn't exist");
    return it->second;
}

shared_ptr<Value> Machine::getVariable(std::string var) {
    return vars[var];
}