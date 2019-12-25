#pragma once

#include <variant>
#include <memory>
#include <map>
#include <string>

using table = std::map<std::string, double>;

class Expr {
public:
    virtual double eval(const table &t)=0;
};

class NumberExpr : public Expr {

public:
    NumberExpr() = default;
    NumberExpr(double value) {this->value = value; }
    double eval(const table &t);
private:
    double value;

};

class IdentifierExpr : public Expr {

public:
    IdentifierExpr() = default;
    IdentifierExpr(std::string name) {this->name = name; }
    double eval(const table &t);
private:
    std::string name;
};

enum class BOpType {
    Plus, Minus ,Mul, Div
};

class BOpExpr : public Expr {
public:
    BOpExpr() = default;
    BOpExpr(BOpType type, std::shared_ptr<Expr> e1, std::shared_ptr<Expr> e2) {
        this->type = type;
        this->e1 = e1;
        this->e2 = e2;
    }
    double eval(const table &t);
private:
    BOpType type;
    std::shared_ptr<Expr> e1, e2;

};

class Statement {
public:
    virtual void exec(table &t) = 0;
};

class AssignStatement : public Statement {
public:
    AssignStatement(std::string id, std::shared_ptr<Expr> e) { this->id = id; this->e = e;}
    void exec(table &t);
private:
    std::string id;
    std::shared_ptr<Expr> e;
};

