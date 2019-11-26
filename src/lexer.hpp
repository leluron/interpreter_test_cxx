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
        LP,
        RP,
        Print,
        End
    };
    Token() = default;
    Token(Kind kind, const char *beg, int length) {
        this->kind = kind; this->lexeme = std::string(beg, length);}

    bool is(Kind kind) const { return this->kind == kind; }
    std::string get() const { return this->lexeme; }
private:
    Kind kind = Kind::None;
    std::string lexeme{};
};

class Lexer {

public:
    Lexer() = default;
    Lexer(const char *beg) { this->beg = beg; }
    void set(const char *beg) { this->beg = beg;}

    Token next();
    Token peek();

private:
    const char * beg = 0;

};