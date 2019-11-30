#pragma once

#include <string>
#include <string_view>

class Token {
public:
    enum class Kind {
        None,
        Identifier,
        Number,
        Assign,
        Plus,
        Minus,
        Mul,
        Div,
        LP,
        RP,
        Semicolon,
        Print,
        End
    };
    Token() = default;
    Token(Kind kind, const char *beg, int length, int line, int column) {
        this->kind_ = kind;
        this->lexeme = std::string(beg, length);
        this->line = line;
        this->column = column;
    }

    Token::Kind kind() const { return kind_; }
    std::string get() const { return this->lexeme; }
private:
    Kind kind_ = Kind::None;
    std::string lexeme{};
    int line, column;
};

class Lexer {

public:
    Lexer() = default;
    Lexer(const char *beg) { this->beg = beg; }
    void set(const char *beg) { this->beg = beg;}

    Token next();
    Token peek();

private:
    void advance();

    const char * beg = 0;

    int currentLine = 0;
    int currentColumn = 0;

};