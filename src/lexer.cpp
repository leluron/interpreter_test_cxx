#include "lexer.hpp"

#include <cstring>
#include <map>

using namespace std;

class Lexer {

public:
    Lexer(const char *beg) { this->cursor = beg; }

    vector<Token> tokenize();

private:
    void advance();
    bool isDigit();
    bool isLetter();
    bool isWhitespace();
    bool isEnd();
    bool isChar(char c);

    void token(Token::Kind kind);

    const char * cursor;
    const char * token_start;
    vector<Token> tokens;
    map<string, Token::Kind> keywords;
    map<char, Token::Kind> atoms;
};

vector<Token> lexer(const char *beg) {
    Lexer lex(beg);
    return lex.tokenize();
}

bool Lexer::isDigit() {
    char c = *cursor;
    return (c>='0' && c<='9');
}

bool Lexer::isLetter() {
    char c = *cursor;
    return (c>='a' && c<='z') || (c>='A' && c<='Z');
}

bool Lexer::isWhitespace() {
    char c = *cursor;
    return c==' ' || c=='\r' || c=='\n' || c=='\t';
}

bool Lexer::isEnd() {
    char c = *cursor;
    return c=='\0';
}

bool Lexer::isChar(char c) {
    return *cursor == c;
}

void Lexer::advance() {
    cursor++;
}

void Lexer::token(Token::Kind kind) {
    tokens.push_back(Token(kind, token_start, cursor-token_start));
}

vector<Token> Lexer::tokenize() {
    keywords["function"] = Token::Kind::Function;
    keywords["nil"] = Token::Kind::Nil;
    atoms['='] = Token::Kind::Assign;
    atoms['+'] = Token::Kind::Plus;
    atoms['-'] = Token::Kind::Minus;
    atoms['*'] = Token::Kind::Mul;
    atoms['/'] = Token::Kind::Div;
    atoms['('] = Token::Kind::LP;
    atoms[')'] = Token::Kind::RP;
    atoms['{'] = Token::Kind::LC;
    atoms['}'] = Token::Kind::RC;
    atoms[';'] = Token::Kind::Semicolon;

    while(true) {
        while (isWhitespace()) advance();
        if (isEnd()) return tokens;
        token_start = cursor;

        if (isDigit()) {
            while (isDigit() || isChar('.')) advance();
            token(Token::Kind::Number);
        }
        else if (isLetter() || isChar('_')) {
            while (isLetter() || isDigit() || isChar('_')) advance();
            auto kw = keywords.find(string(token_start, cursor-token_start));
            if (kw == keywords.end()) token(Token::Kind::Identifier);
            else token(kw->second);
        }
        else {
            auto at = atoms.find(*cursor);
            if (at == atoms.end()) token(Token::Kind::Undefined);
            else token(at->second);
            advance();
        }
    }
}