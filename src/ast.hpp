#pragma once

#include <variant>
#include <memory>
#include <map>
#include <string>

enum class ValueKind {
    Nil, Number
};

class Value {
public:
    virtual ValueKind kind() const = 0;
    virtual std::string toString() const = 0;
};

class ValueNil : public Value {
public:
    ValueKind kind() const {return ValueKind::Nil;}
    std::string toString() const {return "nil";}
};

class ValueNumber : public Value {
public:
    ValueNumber(double num) { this->num = num;}
    ValueKind kind() const {return ValueKind::Number;}
    double value() {return num;}
    std::string toString() const { return std::to_string(num);}
private:
    double num;
};

using table = std::map<std::string, std::shared_ptr<Value>>;

class Expr {
public:
    virtual std::shared_ptr<Value> eval(const table &t)=0;
};

class NilExpr : public Expr {
    std::shared_ptr<Value> eval(const table &t);
};

class NumberExpr : public Expr {

public:
    NumberExpr() = default;
    NumberExpr(double value) {this->value = value; }
    std::shared_ptr<Value> eval(const table &t);
private:
    double value;

};

class IdentifierExpr : public Expr {

public:
    IdentifierExpr() = default;
    IdentifierExpr(std::string name) {this->name = name; }
    std::shared_ptr<Value> eval(const table &t);
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
    std::shared_ptr<Value> eval(const table &t);
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

