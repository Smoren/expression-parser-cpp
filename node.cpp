#include "node.h"

NodeValue::NodeValue(bool boolVal) : boolVal(boolVal) {
    doubleVal = static_cast<double>(boolVal);
}

NodeValue::NodeValue(double doubleVal) : doubleVal(doubleVal) {
    boolVal = static_cast<bool>(doubleVal);
}

bool NodeValue::getBoolVal() const {
    return boolVal;
}

double NodeValue::getDoubleVal() const {
    return doubleVal;
}

Node::Node() {

}

void Node::setIndex(int index) {
    this->index = index;
}

int Node::getIndex() {
    return index;
}

ConstNode::ConstNode(NodeValue value) : value(value) {

}

NodeValue ConstNode::eval(std::map<std::string, NodeValue> vars) {
    return value;
}

VarNode::VarNode(std::string name) : name(name) {

}

NodeValue VarNode::eval(std::map<std::string, NodeValue> vars) {
    if(vars.find(name) == vars.end()) {
        throw std::runtime_error("var is not defined");
    }

    return vars.at(name);
}

ComparisonOperationNode::ComparisonOperationNode(
    const ComparisonOperator& op, const std::shared_ptr<Node>& lNode, const std::shared_ptr<Node>& rNode
) : op(op), lNode(lNode), rNode(rNode) {

}

NodeValue ComparisonOperationNode::eval(std::map<std::string, NodeValue> vars) {
    double diff, lhsVal, rhsVal;
    switch(op) {
        case ComparisonOperator::MORE:
            return NodeValue(lNode->eval(vars).getDoubleVal() > rNode->eval(vars).getDoubleVal());
        case ComparisonOperator::LESS:
            return NodeValue(lNode->eval(vars).getDoubleVal() < rNode->eval(vars).getDoubleVal());
        case ComparisonOperator::EQUAL: {
            diff = std::fabs(lNode->eval(vars).getDoubleVal() - rNode->eval(vars).getDoubleVal());
            return NodeValue(diff < std::numeric_limits<double>::epsilon());
        }
        case ComparisonOperator::NOT_EQUAL: {
            diff = std::fabs(lNode->eval(vars).getDoubleVal() - rNode->eval(vars).getDoubleVal());
            return NodeValue(diff >= std::numeric_limits<double>::epsilon());
        }
        case ComparisonOperator::MORE_EQUAL: {
            lhsVal = lNode->eval(vars).getDoubleVal();
            rhsVal = rNode->eval(vars).getDoubleVal();
            diff = std::fabs(lhsVal - rhsVal);
            return NodeValue(diff < std::numeric_limits<double>::epsilon() || lhsVal > rhsVal);
        }
        case ComparisonOperator::LESS_EQUAL: {
            lhsVal = lNode->eval(vars).getDoubleVal();
            rhsVal = rNode->eval(vars).getDoubleVal();
            diff = std::fabs(lhsVal - rhsVal);
            return NodeValue(diff < std::numeric_limits<double>::epsilon() || lhsVal < rhsVal);
        }
    }
}

LogicalOperationNode::LogicalOperationNode(
    const LogicalOperator& op, const std::shared_ptr<Node>& lNode, const std::shared_ptr<Node>& rNode
) : op(op), lNode(lNode), rNode(rNode) {

}

NodeValue LogicalOperationNode::eval(std::map<std::string, NodeValue> vars) {
    switch(op) {
        case LogicalOperator::OR:
            return NodeValue(lNode->eval(vars).getBoolVal() || rNode->eval(vars).getBoolVal());
        case LogicalOperator::AND:
            return NodeValue(lNode->eval(vars).getBoolVal() && rNode->eval(vars).getBoolVal());
    }
}

AriphmeticOperationNode::AriphmeticOperationNode(
    const AriphmeticOperator& op, const std::shared_ptr<Node>& lNode, const std::shared_ptr<Node>& rNode
) : op(op), lNode(lNode), rNode(rNode) {

}

NodeValue AriphmeticOperationNode::eval(std::map<std::string, NodeValue> vars) {
    switch(op) {
        case AriphmeticOperator::ADD:
            return NodeValue(lNode->eval(vars).getDoubleVal() + rNode->eval(vars).getDoubleVal());
        case AriphmeticOperator::SUB:
            return NodeValue(lNode->eval(vars).getDoubleVal() - rNode->eval(vars).getDoubleVal());
        case AriphmeticOperator::MUL:
            return NodeValue(lNode->eval(vars).getDoubleVal() * rNode->eval(vars).getDoubleVal());
        case AriphmeticOperator::DIV:
            return NodeValue(lNode->eval(vars).getDoubleVal() / rNode->eval(vars).getDoubleVal());
    }
}

std::ostream& operator <<(std::ostream& stream, const NodeValue& nodeValue) {
    return stream << nodeValue.getDoubleVal();
}
