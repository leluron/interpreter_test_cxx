#pragma once

#include <variant>
#include <memory>

enum class ExprKind {
    Number, BOp, Identifier
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

class IdentifierExpr : public Expr {

public:
    ExprKind kind() { return ExprKind::Identifier; }
    IdentifierExpr() = default;
    IdentifierExpr(std::string name) {this->name = name; }
    std::string get() { return name; }
private:
    std::string name;
};

enum class BOpType {
    Plus, Minus ,Mul, Div
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
    Print, Assign
};

class Statement {
public:
    virtual StatementKind kind() = 0;
};

class AssignStatement : public Statement {
public:
    StatementKind kind() { return StatementKind::Assign; }
    AssignStatement(std::string id, std::shared_ptr<Expr> e) { this->id = id; this->e = e;}
    std::shared_ptr<Expr> getExpr() { return e; }
    std::string getId() { return id; }
private:
    std::string id;
    std::shared_ptr<Expr> e;
};

