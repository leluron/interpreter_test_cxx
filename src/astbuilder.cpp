#include "astbuilder.hpp"
#include <sstream>

using namespace std;

shared_ptr<Statement> ASTBuilder::nextStatement() {
    return parseStatement();
}

bool ASTBuilder::expect(Token::Kind kind) {
    return lexer.next().kind() == kind;
}

shared_ptr<Statement> ASTBuilder::parseStatement() {
    Token token = lexer.next();
    switch (token.kind()) {
        case Token::Kind::Print: return parsePrintStatement();
        default: throw runtime_error("Not Supported");
    }
}

shared_ptr<Statement> ASTBuilder::parsePrintStatement() {
    return shared_ptr<Statement>(new PrintStatement(parseExpression()));
}

shared_ptr<Expr> ASTBuilder::parseExpression() {
    auto e = parseFactor();
    return parseExpressionAux(e);
}

shared_ptr<Expr> ASTBuilder::parseExpressionAux(std::shared_ptr<Expr> e) {
    Token token = lexer.peek();
    BOpType type;
    switch (token.kind()) {
        case Token::Kind::Plus: type = BOpType::Plus; break;
        case Token::Kind::Minus: type = BOpType::Minus; break;
        default :
            return e;
    }
    lexer.next();
    return parseExpressionAux(shared_ptr<Expr>(new BOpExpr(type, e, parseFactor())));
}

shared_ptr<Expr> ASTBuilder::parseFactor() {
    auto e = parsePrimary();
    return parseFactorAux(e);
}

shared_ptr<Expr> ASTBuilder::parseFactorAux(std::shared_ptr<Expr> e) {
    Token token = lexer.peek();
    BOpType type;
    switch (token.kind()) {
        case Token::Kind::Mul: type = BOpType::Mul; break;
        case Token::Kind::Div: type = BOpType::Div; break;
        default :
            return e;
    }
    lexer.next();
    return parseExpressionAux(shared_ptr<Expr>(new BOpExpr(type, e, parsePrimary())));
}

shared_ptr<Expr> ASTBuilder::parsePrimary() {
    Token token = lexer.next();
    switch (token.kind()) {
        case Token::Kind::LP: {
            std::shared_ptr<Expr> e = parseExpression();
            if (!expect(Token::Kind::RP)) throw runtime_error("No closing parenthesis");
            return e;
        }
        case Token::Kind::Number: {
            stringstream ss(token.get());
            double number;
            ss >> number;
            return shared_ptr<Expr>(new NumberExpr(number));
        }

        default:
            throw runtime_error("Not Supported");
    }
}