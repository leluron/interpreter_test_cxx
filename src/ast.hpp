#pragma once

#include <variant>
#include <memory>

enum class ExprKind {
    Number, BOp
};

class Expr {
public:
    virtual ExprKind kind() = 0; 
};

class NumberExpr : public Expr {

public:
    ExprKind kind() { return ExprKind::Number; }
    NumberExpr() = default;
    NumberExpr(double value) {this->value = value; }
    double get() { return  value; }
private:
    double value;

};

enum class BOpType {
    Plus, Minus
};

class BOpExpr : public Expr {
public:
    ExprKind kind() { return ExprKind::BOp; }
    BOpExpr() = default;
    BOpExpr(BOpType type, std::shared_ptr<Expr> e1, std::shared_ptr<Expr> e2) {
        this->type = type;
        this->e1 = e1;
        this->e2 = e2;
    }
    BOpType getType() { return type;}
    std::shared_ptr<Expr> getLeft() { return e1;}
    std::shared_ptr<Expr> getRight() { return e2;}
private:
    BOpType type;
    std::shared_ptr<Expr> e1, e2;

};

enum class StatementKind {
    Print
};

class Statement {
public:
    virtual StatementKind kind() = 0;
};

class PrintStatement : public Statement {
public:
    StatementKind kind() { return StatementKind::Print; }
    PrintStatement(std::shared_ptr<Expr> e) { this->e = e;}
    std::shared_ptr<Expr> getExpr() { return e; }
private:
    std::shared_ptr<Expr> e;
};