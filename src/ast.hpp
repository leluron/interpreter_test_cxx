#pragma once

#include <variant>
#include <memory>
#include <map>
#include <string>
#include <vector>

class Expr;
class Statement;

using exprp = std::shared_ptr<Expr>;
using arglist = std::vector<exprp>;
using argNames = std::vector<std::string>;
using statp = std::shared_ptr<Statement>;
using block = std::vector<statp>;

enum class ValueKind {
    Nil, Number, String, Function
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

class ValueString : public Value {
public:
    ValueString(std::string s) { this->s = s;}
    ValueKind kind() const {return ValueKind::String;}
    std::string value() {return s;}
    std::string toString() const { return "\"" + s + "\"";}
private:
    std::string s;
};

class ValueFunction : public Value {
public:
    ValueFunction(argNames args, block body) {
        this->args = args;
        this->body = body;
    }
    ValueKind kind() const {return ValueKind::Function;}
    argNames getArgs() {return args;}
    block getBody() {return body;}
    std::string toString() const { return "function"; }

private:
    argNames args;
    block body;
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
    Plus, Minus ,Mul, Div, Mod, IsEqual, NotEqual, LessEqual, Less, Greater, GreaterEqual, And, Or
};

class BOpExpr : public Expr {
public:
    BOpExpr() = default;
    BOpExpr(BOpType type, exprp e1, exprp e2) {
        this->type = type;
        this->e1 = e1;
        this->e2 = e2;
    }
    std::shared_ptr<Value> eval(const table &t);
private:
    BOpType type;
    exprp e1, e2;

};

enum class UnOpType {
    Plus, Minus, Not
};

class UnOpExpr : public Expr {
public:
    UnOpExpr() = default;
    UnOpExpr(UnOpType type, exprp e) {
        this->type = type;
        this->e = e;
    }
    std::shared_ptr<Value> eval(const table &t);
private:
    UnOpType type;
    exprp e;

};

class StringLiteralExpr : public Expr {
public:
    StringLiteralExpr() = default;
    StringLiteralExpr(std::string literal) {this->literal = literal;}
    std::shared_ptr<Value> eval(const table &t);
private:
    std::string literal;
};

class CallExpr : public Expr {
public:
    CallExpr() = default;
    CallExpr(std::string identifier, arglist args) {
        this->identifier = identifier;
        this->args = args;
    }
    std::shared_ptr<Value> eval(const table &t);
private:
    std::string identifier;
    arglist args;
};

class TernaryExpr : public Expr {
public:
    TernaryExpr() = default;
    TernaryExpr(exprp cond, exprp e1, exprp e2) {
        this->cond = cond;
        this->e1 = e1;
        this->e2 = e2;
    }
    std::shared_ptr<Value> eval(const table &t);
private:
    exprp cond, e1, e2;
};

class FunctionExpr : public Expr {
public:
    FunctionExpr() = default;
    FunctionExpr(argNames args, block body) {
        this->args = args;
        this->body = body;
    }
    std::shared_ptr<Value> eval(const table &t);
private:
    argNames args;
    block body;
};

class Statement {
public:
    virtual void exec(table &t) = 0;
};

class AssignStatement : public Statement {
public:
    AssignStatement(std::string id, exprp e) { this->id = id; this->e = e;}
    void exec(table &t);
private:
    std::string id;
    exprp e;
};

class ConditionStatement : public Statement {
public:
    ConditionStatement(exprp cond, block b1) { this->cond = cond; this->b1 = b1; this->elseb = false;}
    ConditionStatement(exprp cond, block b1, block b2) { this->cond = cond; this->b1 = b1; this->elseb = true; this->b2 = b2;}
    void exec(table &t);
private:
    exprp cond;
    block b1, b2;
    bool elseb;
};

class WhileStatement : public Statement {
public:
    WhileStatement(exprp cond, block body) {
        this->cond = cond;
        this->body = body;
    }
    void exec(table &t);
private:
    exprp cond;
    block body;
};

class ForStatement : public Statement {
public:
    ForStatement(statp init, exprp cond, statp action, block body) {
        this->init = init;
        this->cond = cond;
        this->action = action;
        this->body = body;
    }
    void exec(table &t);
private:
    statp init, action;
    exprp cond;
    block body;
};

class ReturnStatement : public Statement {
public:
    ReturnStatement(exprp e) { this->e = e;}
    void exec(table &t);
private:
    exprp e;
};
