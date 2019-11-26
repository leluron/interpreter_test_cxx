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
    std::shared_ptr<Expr> parseExpression();

    Lexer lexer;
};