#include "machine.hpp"
#include "lexer.hpp"
#include "astbuilder.hpp"

#include <iostream>

using namespace std;

void Machine::interpret(string expr) {
    Lexer lexer;
    lexer.set(expr.c_str());
    auto st = ASTBuilder(lexer).nextStatement();
    execStatement(st);
}

void Machine::execStatement(std::shared_ptr<Statement> s) {
    if (s->kind() == StatementKind::Print) {
        cout << eval(dynamic_pointer_cast<PrintStatement>(s)->getExpr()) << endl;
    } else {
        throw runtime_error("Not Supported");
    }
}

double Machine::eval(std::shared_ptr<Expr> e) {
    if (e->kind() == ExprKind::Number) {
        return dynamic_pointer_cast<NumberExpr>(e)->get();
    } else if (e->kind() == ExprKind::BOp) {
        auto b = dynamic_pointer_cast<BOpExpr>(e);
        double l = eval(b->getLeft ());
        double r = eval(b->getRight());
        if (b->getType() == BOpType::Plus) return l+r;
        else if (b->getType() == BOpType::Minus) return l-r;
        else throw runtime_error("Not Supported");
    } else {
        throw runtime_error("Not Supported");
    }
}
