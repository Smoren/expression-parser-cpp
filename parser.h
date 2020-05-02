#pragma once

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>
#include "node.h"

enum class TokenType {
    NUMBER,
    VARIABLE,
    BRACKET_OPEN,
    BRACKET_CLOSE,
    OP_ARIPHMETIC_ADD,
    OP_ARIPHMETIC_SUB,
    OP_ARIPHMETIC_MUL,
    OP_ARIPHMETIC_DIV,
    OP_LOGICAL_AND,
    OP_LOGICAL_OR,
    OP_COMPARISON_MORE,
    OP_COMPARISON_LESS,
    OP_COMPARISON_EQUAL,
    OP_COMPARISON_NOT_EQUAL,
    OP_COMPARISON_MORE_EQUAL,
    OP_COMPARISON_LESS_EQUAL,
};

enum class TokenPriority {
    OPERAND,
    COMPARISON,
    ADD,
    MUL,
    BRACKET
};

std::ostream& operator <<(std::ostream& stream, const TokenType& item);
std::ostream& operator <<(std::ostream& stream, const TokenPriority& item);

class Token {
public:
    explicit Token(std::string value, int depthLevel, int index);
    std::string getValue() const;
    TokenType getType() const;
    int getPriority() const;
    int getDefaultPriority() const;
    void boostPriority(int value);
    void boostPriority(Token& op);
    int getConstOperandCount() const;
    double getDoubleValue() const;
    std::string getStringValue() const;
    int getIndex() const;
protected:
    std::string value;
    TokenType type;
    int priority;
    int constOperandCount;
    int index;
    Token* linkedOperator;

    static const std::map<TokenType, TokenPriority> priorityMap;
};

std::ostream& operator <<(std::ostream& stream, const Token& item);

class Expression {
public:
    explicit Expression(std::string input);
    void tokenize();
    void parse();
    NodeValue eval(std::map<std::string, NodeValue> vars);
    void addToken(std::string token, int& depthLevel, int index);
    std::string getInput() const;
    const std::vector<Token>& getTokens() const;
    const std::vector<Token>& getOperands() const;
    const std::vector<Token>& getOperators() const;
protected:
    std::string input;
    std::vector<Token> tokens;
    std::vector<Token> operands;
    std::vector<Token> operators;
    std::stack< std::shared_ptr<Node> > nodes;
};

std::ostream& operator <<(std::ostream& stream, const Expression& item);
