#include "lexer.hpp"

#include <cstring>

bool isDigit(char c) {
    return (c>='0' && c<='9');
}

bool isLetter(char c) {
    return (c>='a' && c<='z') || (c>='A' && c<='Z');
}

bool isNumberStart(char c) {
    return isDigit(c) || c=='.';
}

bool isNumberFollow(char c) {
    return isDigit(c) || c=='.';
}

bool isIdentifierStart(char c) {
    return isLetter(c) || c == '_';
}

bool isIdentifierFollow(char c) {
    return isLetter(c) || isDigit(c);
}

bool isSpace(char c) {
    return c==' ' || c=='\r' || c=='\n' || c=='\t';
}

bool isEnd(char c) {
    return c=='\0';
}

void Lexer::advance() {
    if (*beg == '\n') {
        currentLine++;
        currentColumn = 0;
    }
    beg++;
    currentColumn++;
}

Token Lexer::next() {
    while (isSpace(*beg)) beg++;
    if (isEnd(*beg)) return Token(Token::Kind::End, beg, 0, currentLine, currentColumn);
    Token::Kind k = Token::Kind::None; 
    const char *start = beg;

    if (isNumberStart(*beg)) {
        k = Token::Kind::Number;
        while (isNumberFollow(*beg)) advance();
    }
    else if (isIdentifierStart(*beg)) {
        k = Token::Kind::Identifier;
        while (isIdentifierFollow(*beg)) advance();
    }
    else {
             if (*beg == '=') k = Token::Kind::Assign;
        else if (*beg == '+') k = Token::Kind::Plus;
        else if (*beg == '-') k = Token::Kind::Minus;
        else if (*beg == '*') k = Token::Kind::Mul;
        else if (*beg == '/') k = Token::Kind::Div;
        else if (*beg == '(') k = Token::Kind::LP;
        else if (*beg == ')') k = Token::Kind::RP;
        else if (*beg == ';') k = Token::Kind::Semicolon;
        beg++;
    }

    int length = beg-start;
    if (strncmp(start, "print", length) == 0) k = Token::Kind::Print;
    return Token(k, start, length, currentLine, currentColumn);
}

Token Lexer::peek() {
    auto temp = beg;
    auto tok = next();
    beg = temp;
    return tok;
}
