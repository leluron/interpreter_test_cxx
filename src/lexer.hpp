#pragma once

#include <string>
#include <vector>

class Token {
public:
    enum class Kind {
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
        Function,
        End
    };
    Token() = default;
    Token(Kind kind, const char *beg, int length, int line, int column) {
        this->kind_ = kind;
        this->beg = beg;
        this->length = length;
        this->line = line;
        this->column = column;
    }

    Token::Kind kind() const { return kind_; }
    std::string get() const { return std::string(beg, length); }
private:
    Kind kind_ = Kind::End;
    const char *beg;
    int length;
    int line, column;
};

std::vector<Token> lexer(const char *beg);