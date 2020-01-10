#pragma once

#include "ast.hpp"
#include "lexer.hpp"

#include <vector>

class ASTBuilder {
public:
    ASTBuilder(std::vector<Token> t) { tokens = t;}
    block parseFile();

private:
    statp parseStatement();
    statp parseStatementAux(std::string id);
    statp parseIf();
    statp parseWhile();
    statp parseFor();
    statp parseReturn();
    statp parseAssignStatement(std::string id);

    std::vector<std::string> parseArgList();

    block parseBlock();

    exprp parseExpression();
    exprp parseTernary();
    exprp parseFunction();
    exprp parseLogicOr();
    exprp parseLogicOrAux(exprp e);
    exprp parseLogicAnd();
    exprp parseLogicAndAux(exprp e);
    exprp parseComp();
    exprp parseCompAux(exprp e);
    exprp parseRel();
    exprp parseRelAux(exprp e);
    exprp parseTerm();
    exprp parseTermAux(exprp e);
    exprp parseFactor();
    exprp parseFactorAux(exprp e);
    exprp parsePrimary();
    exprp parsePrimaryAux(UnOpType t);
    exprp parseElement();

    Token next();
    Token peek();

    bool expect(Token::Kind kind);

    std::vector<Token> tokens;
    size_t index = 0;
};