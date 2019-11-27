#include "astbuilder.hpp"
#include <sstream>

using namespace std;

shared_ptr<Statement> ASTBuilder::nextStatement() {
    return parseStatement();
}

shared_ptr<Statement> ASTBuilder::parseStatement() {
    Token token = lexer.next();
    switch (token.kind()) {
        case Token::Kind::Print:
            std::shared_ptr<Expr> e = parseExpression();
            return shared_ptr<Statement>(new PrintStatement(e));
        default:
            throw runtime_error("Not Supported");
    }
}

shared_ptr<Expr> ASTBuilder::parseExpression() {
    Token token = lexer.next();
    switch (token.kind()) {
        case Token::Kind::LP:
            std::shared_ptr<Expr> e = parseExpression();
            if (!lexer.next().is(Token::Kind::RP)) throw runtime_error("No closing parenthesis");
            return e;
        case Token::Kind::Number:
            stringstream ss(token.get());
            double number;
            ss >> number;
            return shared_ptr<Expr>(new NumberExpr(number));

        default:
            throw runtime_error("Not Supported");
    }
}