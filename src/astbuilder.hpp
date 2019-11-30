#pragma once

#include "ast.hpp"
#include "lexer.hpp"

#include "lexer.hpp"
#include "ast.hpp"

#include <vector>

class ASTBuilder {
public:
    ASTBuilder(Lexer l) { lexer = l;}
    std::vector<std::shared_ptr<Statement>> parseStatementList();

private:
    std::shared_ptr<Statement> parseStatement();
    std::shared_ptr<Statement> parseStatementAux(std::string id);
    std::shared_ptr<Statement> parsePrintStatement();
    std::shared_ptr<Statement> parseAssignStatement(std::string id);

    std::shared_ptr<Expr> parseExpression();
    std::shared_ptr<Expr> parseExpressionAux(std::shared_ptr<Expr> e);
    std::shared_ptr<Expr> parseFactor();
    std::shared_ptr<Expr> parseFactorAux(std::shared_ptr<Expr> e);
    std::shared_ptr<Expr> parsePrimary();

    bool expect(Token::Kind kind);

    Lexer lexer;
};