#include "machine.hpp"
#include "lexer.hpp"
#include "astbuilder.hpp"

#include <iostream>

using namespace std;

void Machine::interpret(string expr) {
    auto tokens = lexer(expr.c_str());
    auto st = ASTBuilder(tokens).parseStatementList();
    for (auto s : st) {
        execStatement(s);
    }
}

void Machine::execStatement(std::shared_ptr<Statement> s) {
    switch (s->kind()) {
        case StatementKind::Assign: execAssign(dynamic_pointer_cast<AssignStatement>(s)); break;
        default: throw runtime_error("Not Supported");
    }
}

void Machine::execAssign(std::shared_ptr<AssignStatement> s) {
    vars[s->getId()] = eval(s->getExpr());
}

double Machine::eval(std::shared_ptr<Expr> e) {
    if (e->kind() == ExprKind::Number) {
        return dynamic_pointer_cast<NumberExpr>(e)->get();
    } else if (e->kind() == ExprKind::BOp) {
        auto b = dynamic_pointer_cast<BOpExpr>(e);
        double l = eval(b->getLeft ());
        double r = eval(b->getRight());
        switch (b->getType()) {
            case BOpType::Plus : return l+r;
            case BOpType::Minus: return l-r;
            case BOpType::Mul  : return l*r;
            case BOpType::Div  : return l/r;
            default: throw runtime_error("Not Supported");
        }
    } else if (e->kind() == ExprKind::Identifier) {
        auto name = dynamic_pointer_cast<IdentifierExpr>(e)->get();
        auto it = vars.find(name);
        if (it == vars.end()) throw runtime_error("Variable doesn't exist");
        return it->second;
    } else {
        throw runtime_error("Not Supported");
    }
}

double Machine::getVariable(std::string var) {
    return vars[var];
}