#pragma once

#include "ast.hpp"
#include "lexer.hpp"

#include "lexer.hpp"
#include "ast.hpp"

class ASTBuilder {
public:
    ASTBuilder(Lexer l) { lexer = l;}
    std::shared_ptr<Statement> nextStatement();

private:
    std::shared_ptr<Statement> parseStatement();
    std::shared_ptr<Statement> parsePrintStatement();

    std::shared_ptr<Expr> parseExpression();
    std::shared_ptr<Expr> parseExpressionAux(std::shared_ptr<Expr> e);
    std::shared_ptr<Expr> parseFactor();
    std::shared_ptr<Expr> parseFactorAux(std::shared_ptr<Expr> e);
    std::shared_ptr<Expr> parsePrimary();

    bool expect(Token::Kind kind);

    Lexer lexer;
};