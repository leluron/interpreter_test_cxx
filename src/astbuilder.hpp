#pragma once

#include "ast.hpp"
#include "lexer.hpp"

#include <vector>

class ASTBuilder {
public:
    ASTBuilder(std::vector<Token> t) { tokens = t;}
    std::vector<std::shared_ptr<Statement>> parseStatementList();

private:
    std::shared_ptr<Statement> parseStatement();
    std::shared_ptr<Statement> parseStatementAux(std::string id);
    std::shared_ptr<Statement> parseAssignStatement(std::string id);

    std::shared_ptr<Expr> parseExpression();
    std::shared_ptr<Expr> parseExpressionAux(std::shared_ptr<Expr> e);
    std::shared_ptr<Expr> parseFactor();
    std::shared_ptr<Expr> parseFactorAux(std::shared_ptr<Expr> e);
    std::shared_ptr<Expr> parsePrimary();

    Token next();
    Token peek();

    bool expect(Token::Kind kind);

    std::vector<Token> tokens;
    size_t index = 0;
};