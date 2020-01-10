#include "astbuilder.hpp"
#include <sstream>

using namespace std;

Token ASTBuilder::next() {
    if (index >= tokens.size()) throw runtime_error("Reached end of tokens");
    return tokens[index++];
}

Token ASTBuilder::peek() {
    auto i = index;
    Token t = next();
    index = i;
    return t;
}

block ASTBuilder::parseFile() {
    return parseBlock();
}

bool ASTBuilder::expect(Token::Kind kind) {
    return next().kind() == kind;
}

statp ASTBuilder::parseStatement() {
    Token token = next();
    switch (token.kind()) {
        case Token::Kind::Identifier: return parseStatementAux(token.get());
        case Token::Kind::If : return parseIf();
        case Token::Kind::While: return parseWhile();
        case Token::Kind::For: return parseFor();
        case Token::Kind::Return: return parseReturn();
        default: throw runtime_error("Not Supported");
    }
}

statp ASTBuilder::parseStatementAux(string id) {
    Token token = next();
    switch (token.kind()) {
        case Token::Kind::Assign: return parseAssignStatement(id);
        default: throw runtime_error("Not Supported");
    }
}

statp ASTBuilder::parseAssignStatement(string id) {
    return statp(new AssignStatement(id, parseExpression())) ;
}

statp ASTBuilder::parseIf() {
    auto cond = parseExpression();
    if (!expect(Token::Kind::Then)) throw runtime_error("expected then");
    auto b1 = parseBlock();
    auto k = next().kind();
    if (k == Token::Kind::End) return statp(new ConditionStatement(cond, b1));
    else if (k == Token::Kind::Else) {
        auto b2 = parseBlock();
        if (!expect(Token::Kind::End)) throw runtime_error("expected end");
        return statp(new ConditionStatement(cond, b1, b2));
    }
    else if (k == Token::Kind::ElseIf) {
        auto s = parseIf();
        return statp(new ConditionStatement(cond, b1, {s}));
    }
    else throw runtime_error("unexpected symbol for block end");
}

statp ASTBuilder::parseWhile() {
    auto cond = parseExpression();
    if (!expect(Token::Kind::Do)) throw runtime_error("expected do");
    auto b = parseBlock();
    if (!expect(Token::Kind::End)) throw runtime_error("expected end");
    return statp(new WhileStatement(cond, b));
}

statp ASTBuilder::parseFor() {
    if (!expect(Token::Kind::LP)) throw runtime_error("expected (");
    auto init = parseStatement();
    if (!expect(Token::Kind::Comma)) throw runtime_error("expected ,");
    auto cond = parseExpression();
    if (!expect(Token::Kind::Comma)) throw runtime_error("expected ,");
    auto action = parseStatement();
    if (!expect(Token::Kind::RP)) throw runtime_error("expected )");
    auto body = parseBlock();
    if (!expect(Token::Kind::End)) throw runtime_error("expected end");
    return statp(new ForStatement(init, cond, action, body));
}

statp ASTBuilder::parseReturn() {
    return statp(new ReturnStatement(parseExpression()));
}

exprp ASTBuilder::parseExpression() {
    Token token = peek();
    switch (token.kind()) {
        case Token::Kind::If: return parseTernary();
        case Token::Kind::Function: return parseFunction();
        default: return parseLogicOr();
    }
}

exprp ASTBuilder::parseTernary() {
    next();
    auto cond = parseExpression();
    if (!expect(Token::Kind::Then)) throw runtime_error("expected then");
    auto e1 = parseExpression();
    if (!expect(Token::Kind::Else)) throw runtime_error("expected else");
    auto e2 = parseExpression();
    return exprp(new TernaryExpr(cond, e1, e2));
}

vector<string> ASTBuilder::parseArgList() {
    auto t = next();
    if (t.kind() != Token::Kind::Identifier) throw runtime_error("expected identifier as arg");
    vector<string> args = {t.get()};
    Token t2 = peek();
    while (t2.kind() == Token::Kind::Comma) {
        next();
        auto t = next();
        if (t.kind() != Token::Kind::Identifier) throw runtime_error("expected identifier as arg");
        args.push_back(t.get());
        t2 = peek();
    }
    return args;
}

block ASTBuilder::parseBlock() {
    block list = {};
    while (true) {
        switch (peek().kind()) {
            case Token::Kind::Eof:
            case Token::Kind::End:
            case Token::Kind::Else:
            case Token::Kind::ElseIf:
                return list;
            default:
                list.push_back(parseStatement());
        }
    }
    return list;
}


exprp ASTBuilder::parseFunction() {
    next();
    expect(Token::Kind::LP);
    auto args = parseArgList();
    expect(Token::Kind::RP);
    auto body = parseBlock();
    if (!expect(Token::Kind::End)) throw ("expected end");
    return exprp(new FunctionExpr(args, body));
}

exprp ASTBuilder::parseLogicOr() {
    auto e = parseLogicAnd();
    return parseLogicOrAux(e);
}

exprp ASTBuilder::parseLogicOrAux(exprp e) {
    Token token = peek();
    BOpType type;
    switch (token.kind()) {
        case Token::Kind::Or: type = BOpType::Or; break;
        default :
            return e;
    }
    next();
    return parseLogicOrAux(exprp(new BOpExpr(type, e, parseLogicAnd())));
}

exprp ASTBuilder::parseLogicAnd() {
    auto e = parseComp();
    return parseLogicAndAux(e);
}

exprp ASTBuilder::parseLogicAndAux(exprp e) {
    Token token = peek();
    BOpType type;
    switch (token.kind()) {
        case Token::Kind::And: type = BOpType::And; break;
        default :
            return e;
    }
    next();
    return parseLogicAndAux(exprp(new BOpExpr(type, e, parseComp())));
}

exprp ASTBuilder::parseComp() {
    auto e = parseRel();
    return parseCompAux(e);
}

exprp ASTBuilder::parseCompAux(exprp e) {
    Token token = peek();
    BOpType type;
    switch (token.kind()) {
        case Token::Kind::IsEqual: type = BOpType::IsEqual; break;
        case Token::Kind::NotEqual: type = BOpType::NotEqual; break;
        default :
            return e;
    }
    next();
    return parseCompAux(exprp(new BOpExpr(type, e, parseRel())));
}

exprp ASTBuilder::parseRel() {
    auto e = parseTerm();
    return parseRelAux(e);
}

exprp ASTBuilder::parseRelAux(exprp e) {
    Token token = peek();
    BOpType type;
    switch (token.kind()) {
        case Token::Kind::LessEqual: type = BOpType::LessEqual; break;
        case Token::Kind::GreaterEqual: type = BOpType::GreaterEqual; break;
        case Token::Kind::Less: type = BOpType::Less; break;
        case Token::Kind::Greater: type = BOpType::Greater; break;
        default :
            return e;
    }
    next();
    return parseRelAux(exprp(new BOpExpr(type, e, parseTerm())));
}

exprp ASTBuilder::parseTerm() {
    auto e = parseFactor();
    return parseTermAux(e);
}

exprp ASTBuilder::parseTermAux(exprp e) {
    Token token = peek();
    BOpType type;
    switch (token.kind()) {
        case Token::Kind::Plus: type = BOpType::Plus; break;
        case Token::Kind::Minus: type = BOpType::Minus; break;
        default :
            return e;
    }
    next();
    return parseTermAux(exprp(new BOpExpr(type, e, parseFactor())));
}

exprp ASTBuilder::parseFactor() {
    auto e = parsePrimary();
    return parseFactorAux(e);
}

exprp ASTBuilder::parseFactorAux(exprp e) {
    Token token = peek();
    BOpType type;
    switch (token.kind()) {
        case Token::Kind::Mul: type = BOpType::Mul; break;
        case Token::Kind::Div: type = BOpType::Div; break;
        case Token::Kind::Mod: type = BOpType::Mod; break;
        default :
            return e;
    }
    next();
    return parseFactorAux(exprp(new BOpExpr(type, e, parsePrimary())));
}

exprp ASTBuilder::parsePrimary() {
    Token t = peek();
    switch (t.kind()) {
        case Token::Kind::Minus: parsePrimaryAux(UnOpType::Minus);
        case Token::Kind::Plus: parsePrimaryAux(UnOpType::Plus);
        case Token::Kind::Not: parsePrimaryAux(UnOpType::Not);
        default: return parseElement();
    }
}

exprp ASTBuilder::parsePrimaryAux(UnOpType un) {
    next();
    return exprp(new UnOpExpr(un, parsePrimary()));
}

exprp ASTBuilder::parseElement() {
    Token token = next();
    switch (token.kind()) {
        case Token::Kind::LP: {
            exprp e = parseExpression();
            if (!expect(Token::Kind::RP)) throw runtime_error("No closing parenthesis");
            return e;
        }
        case Token::Kind::Number: {
            stringstream ss(token.get());
            double number;
            ss >> number;
            return exprp(new NumberExpr(number));
        }
        case Token::Kind::Identifier: {
            return exprp(new IdentifierExpr(token.get()));
        }
        case Token::Kind::StringLiteral: {
            return exprp(new StringLiteralExpr(token.get()));
        }

        default:
            throw runtime_error("Not Supported");
    }
}