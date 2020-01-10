#pragma once

#include <string>
#include <vector>

class Token {
public:
    enum class Kind {
        Undefined,
        Eof,
        Identifier,
        Number,
        Assign,
        Plus,
        Minus,
        Mul,
        Div,
        Mod,
        LP,
        RP,
        LC,
        RC,
        Semicolon,
        Function,
        Nil,
        If,
        Then,
        ElseIf,
        End,
        Else,
        While,
        Do,
        For,
        Comma,
        Or,
        And,
        IsEqual,
        NotEqual,
        LessEqual,
        Less,
        Greater,
        GreaterEqual,
        StringLiteral,
        Return,
        Not
    };
    Token() = default;
    Token(Kind kind, const char *beg, int length) {
        this->kind_ = kind;
        this->beg = beg;
        this->length = length;
    }

    Token::Kind kind() const { return kind_; }
    std::string get() const { return std::string(beg, length); }
private:
    Kind kind_;
    const char *beg;
    int length;
};

std::vector<Token> lexer(const char *beg);