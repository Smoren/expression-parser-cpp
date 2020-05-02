#pragma once
#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <limits>
#include <cmath>

enum class ComparisonOperator {
    MORE, LESS, EQUAL, NOT_EQUAL, MORE_EQUAL, LESS_EQUAL
};

enum class LogicalOperator {
    OR, AND
};

enum class AriphmeticOperator {
    ADD, SUB, MUL, DIV
};

class NodeValue {
public:
    explicit NodeValue(bool boolVal);
    explicit NodeValue(double doubleVal);
    bool getBoolVal() const;
    double getDoubleVal() const;
protected:
    bool boolVal;
    double doubleVal;
};

class Node {
public:
    explicit Node();
    virtual NodeValue eval(std::map<std::string, NodeValue> vars) = 0;
    void setIndex(int index);
    int getIndex();
protected:
    int index;
};

class ConstNode : public Node {
public:
    explicit ConstNode(NodeValue value);
    virtual NodeValue eval(std::map<std::string, NodeValue> vars);
protected:
    NodeValue value;
};

class VarNode : public Node {
public:
    explicit VarNode(std::string name);
    virtual NodeValue eval(std::map<std::string, NodeValue> vars);
protected:
    std::string name;
};

class ComparisonOperationNode : public Node {
public:
    explicit ComparisonOperationNode(const ComparisonOperator& op, const std::shared_ptr<Node>& lNode, const std::shared_ptr<Node>& rNode);
    virtual NodeValue eval(std::map<std::string, NodeValue> vars) override;
protected:
    const ComparisonOperator op; // почему нельзя по ссылке???
    const std::shared_ptr<Node> lNode;
    const std::shared_ptr<Node> rNode;
};

class LogicalOperationNode : public Node {
public:
    explicit LogicalOperationNode(const LogicalOperator& op, const std::shared_ptr<Node>& lNode, const std::shared_ptr<Node>& rNode);
    virtual NodeValue eval(std::map<std::string, NodeValue> vars) override;
protected:
    const LogicalOperator op; // почему нельзя по ссылке???
    const std::shared_ptr<Node> lNode;
    const std::shared_ptr<Node> rNode;
};

class AriphmeticOperationNode : public Node {
public:
    explicit AriphmeticOperationNode(const AriphmeticOperator& op, const std::shared_ptr<Node>& lNode, const std::shared_ptr<Node>& rNode);
    virtual NodeValue eval(std::map<std::string, NodeValue> vars) override;
protected:
    const AriphmeticOperator op; // почему нельзя по ссылке???
    const std::shared_ptr<Node> lNode;
    const std::shared_ptr<Node> rNode;
};

std::ostream& operator <<(std::ostream& stream, const NodeValue& nodeValue);
