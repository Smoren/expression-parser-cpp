#include <parser.h>

std::ostream& operator <<(std::ostream& stream, const TokenType& item) {
    switch(item) {
        case TokenType::NUMBER: stream << "NUMBER"; break;
        case TokenType::VARIABLE: stream << "VARIABLE"; break;
        case TokenType::BRACKET_OPEN: stream << "BRACKET_OPEN"; break;
        case TokenType::BRACKET_CLOSE: stream << "BRACKET_CLOSE"; break;
        case TokenType::OP_ARIPHMETIC_ADD: stream << "OP_ADD"; break;
        case TokenType::OP_ARIPHMETIC_SUB: stream << "OP_SUB"; break;
        case TokenType::OP_ARIPHMETIC_MUL: stream << "OP_MUL"; break;
        case TokenType::OP_ARIPHMETIC_DIV: stream << "OP_DIV"; break;
        case TokenType::OP_COMPARISON_MORE: stream << "OP_MORE"; break;
        case TokenType::OP_COMPARISON_LESS: stream << "OP_LESS"; break;
        case TokenType::OP_COMPARISON_EQUAL: stream << "OP_EQUAL"; break;
        case TokenType::OP_COMPARISON_NOT_EQUAL: stream << "OP_NOT_EQUAL"; break;
        case TokenType::OP_COMPARISON_MORE_EQUAL: stream << "OP_MORE_EQUAL"; break;
        case TokenType::OP_COMPARISON_LESS_EQUAL: stream << "OP_LESS_EQUAL"; break;
        case TokenType::OP_LOGICAL_AND: stream << "OP_AND"; break;
        case TokenType::OP_LOGICAL_OR: stream << "OP_OR"; break;
        default: {
            throw std::runtime_error("incomplete output of TokenType");
        }
    }
}

std::ostream& operator <<(std::ostream& stream, const TokenPriority& item) {
    return stream << static_cast<int>(item);
}

Token::Token(std::string value, int depthLevel, int index) : value(value), priority(0), constOperandCount(0), index(index) {
    if(value == "(") {
        type = TokenType::BRACKET_OPEN;
    } else if(value == ")") {
        type = TokenType::BRACKET_CLOSE;
    } else if(value == "+") {
        type = TokenType::OP_ARIPHMETIC_ADD;
    } else if(value == "-") {
        type = TokenType::OP_ARIPHMETIC_SUB;
    } else if(value == "*") {
        type = TokenType::OP_ARIPHMETIC_MUL;
    } else if(value == "/") {
        type = TokenType::OP_ARIPHMETIC_DIV;
    } else if(value == ">") {
        type = TokenType::OP_COMPARISON_MORE;
    } else if(value == "<") {
        type = TokenType::OP_COMPARISON_LESS;
    } else if(value == "=") {
        type = TokenType::OP_COMPARISON_EQUAL;
    } else if(value == "!=") {
        type = TokenType::OP_COMPARISON_NOT_EQUAL;
    } else if(value == ">=") {
        type = TokenType::OP_COMPARISON_MORE_EQUAL;
    } else if(value == "<=") {
        type = TokenType::OP_COMPARISON_LESS_EQUAL;
    } else if(value == "AND") {
        type = TokenType::OP_LOGICAL_AND;
    } else if(value == "OR") {
        type = TokenType::OP_LOGICAL_OR;
    } else {
        long digitsCount = std::count_if(value.begin(), value.end(), [](unsigned char c) {
            return std::isdigit(c);
        });
        long lettersCount = std::count_if(value.begin(), value.end(), [](unsigned char c) {
            return (c >= 'a' && c <= 'z') || c == '_';
        });
        long pointCount = std::count(value.begin(), value.end(), '.');

        if(pointCount <= 1 && static_cast<unsigned long>(digitsCount+pointCount) == value.size()) {
            type = TokenType::NUMBER;
        } else if(static_cast<unsigned long>(digitsCount+lettersCount) == value.size() && !std::isdigit(value[0])) {
            type = TokenType::VARIABLE;
        } else {
            std::stringstream ss;
            ss << "unknown token " << value;
            throw std::runtime_error(ss.str());
        }
    }

    priority = getDefaultPriority() + 10*depthLevel;
}

std::string Token::getValue() const {
    return value;
}

TokenType Token::getType() const {
    return type;
}

int Token::getPriority() const {
    return priority;
}

int Token::getDefaultPriority() const {
    return static_cast<int>(Token::priorityMap.at(type));
}

void Token::boostPriority(int value) {
    priority += value;
}

void Token::boostPriority(Token& op) {
    if(op.priority > priority) {
        op.constOperandCount++;

        if(linkedOperator != nullptr) {
            (*linkedOperator).constOperandCount--;
        }

        linkedOperator = &op;
        priority = op.priority;
    }
}

int Token::getConstOperandCount() const {
    return constOperandCount;
}

double Token::getDoubleValue() const {
    return std::stod(value);
}

std::string Token::getStringValue() const {
    return value;
}

int Token::getIndex() const {
    return index;
}

const std::map<TokenType, TokenPriority> Token::priorityMap = {
    {TokenType::NUMBER, TokenPriority::OPERAND},
    {TokenType::VARIABLE, TokenPriority::OPERAND},
    {TokenType::BRACKET_OPEN, TokenPriority::BRACKET},
    {TokenType::BRACKET_CLOSE, TokenPriority::BRACKET},
    {TokenType::OP_ARIPHMETIC_ADD, TokenPriority::ADD},
    {TokenType::OP_ARIPHMETIC_SUB, TokenPriority::ADD},
    {TokenType::OP_ARIPHMETIC_MUL, TokenPriority::MUL},
    {TokenType::OP_ARIPHMETIC_DIV, TokenPriority::MUL},
    {TokenType::OP_LOGICAL_AND, TokenPriority::MUL},
    {TokenType::OP_LOGICAL_OR, TokenPriority::ADD},
    {TokenType::OP_COMPARISON_MORE, TokenPriority::COMPARISON},
    {TokenType::OP_COMPARISON_LESS, TokenPriority::COMPARISON},
    {TokenType::OP_COMPARISON_EQUAL, TokenPriority::COMPARISON},
    {TokenType::OP_COMPARISON_NOT_EQUAL, TokenPriority::COMPARISON},
    {TokenType::OP_COMPARISON_MORE_EQUAL, TokenPriority::COMPARISON},
    {TokenType::OP_COMPARISON_LESS_EQUAL, TokenPriority::COMPARISON},
};

std::ostream& operator <<(std::ostream& stream, const Token& item) {
    return stream << "'" << item.getValue() << "' (" << item.getType() << ", " << item.getPriority() << ", " << item.getConstOperandCount() << ") " << "#" << item.getIndex();
}

Expression::Expression(std::string input) : input(input) {

}

void Expression::addToken(std::string token, int& depthLevel, int index) {
    auto t = Token(token, depthLevel, index);

    if(t.getType() == TokenType::BRACKET_OPEN) {
        ++depthLevel;
    } else if(t.getType() == TokenType::BRACKET_CLOSE) {
        --depthLevel;
    } else {
        if(t.getDefaultPriority() == static_cast<int>(TokenPriority::OPERAND)) {
            if(operators.size()) {
                t.boostPriority(operators.back());
            }
            operands.push_back(t);
        } else {
            if(operands.size()) {
                operands.back().boostPriority(t);
            }
            operators.push_back(t);
        }
    }
    tokens.push_back(t);
}

void Expression::tokenize() {
    std::stringstream buf;
    int depthLevel = 0;
    int index = 0;

    for(char ch : input) {
        switch(ch) {
            case ' ':
                continue;
            case '(':
            case ')':
            case '+':
            case '-':
            case '*':
            case '/':
            case '>':
            case '<':
            case '=': {
                if(buf.str().size()) {
                    addToken(buf.str(), depthLevel, index);
                    buf.str("");
                }

                buf << ch;
                addToken(buf.str(), depthLevel, index);
                buf.str("");
                break;
            }
            default: {
                buf << ch;
            }
        }

        ++index;
    }

    if(buf.str().size()) {
        addToken(buf.str(), depthLevel, index);
        buf.str("");
    }

    if(depthLevel != 0) {
        throw std::runtime_error("depth level error");
    }

    std::stable_sort(operands.begin(), operands.end(), [](const Token& lhs, const Token& rhs) {
        if(lhs.getPriority() == rhs.getPriority()) {
            return lhs.getIndex() > rhs.getIndex();
        }
        return lhs.getPriority() < rhs.getPriority();
    });

    std::stable_sort(operators.begin(), operators.end(), [](const Token& lhs, const Token& rhs) {
        if(lhs.getPriority() == rhs.getPriority()) {
            return lhs.getIndex() > rhs.getIndex();
        }
        return lhs.getPriority() < rhs.getPriority();
    });
}

void Expression::parse() {
    tokenize();

    std::cout << *this << std::endl;

    while(operators.size()) {
        std::vector< std::shared_ptr<Node> > bufNodes;
        auto& op = operators.back();
        operators.pop_back();

        int constOperandsCount = op.getConstOperandCount();

        if(constOperandsCount > static_cast<int>(operands.size()) || (2-constOperandsCount) > static_cast<int>(nodes.size())) {
            throw std::runtime_error("something went wrong");
        }

        for(int i=0; i<constOperandsCount; i++) {
            auto& operand = operands.back();
            operands.pop_back();

            if(operand.getType() == TokenType::NUMBER) {
                bufNodes.push_back(std::shared_ptr<ConstNode>(new ConstNode(NodeValue(operand.getDoubleValue()))));
            } else {
                bufNodes.push_back(std::shared_ptr<VarNode>(new VarNode(operand.getStringValue())));
            }
            bufNodes.back()->setIndex(operand.getIndex());
        }
        for(int i=0; i<2-constOperandsCount; i++) {
            auto operand = nodes.top();
            nodes.pop();

            bufNodes.push_back(operand);
        }

        std::sort(bufNodes.begin(), bufNodes.end(), [](const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) {
            return lhs->getIndex() < rhs->getIndex();
        });

        std::shared_ptr<Node> node;
        switch(op.getType()) {
            case TokenType::OP_LOGICAL_OR:
                node = std::shared_ptr<Node>(new LogicalOperationNode(LogicalOperator::OR, bufNodes[0], bufNodes[1]));
                break;
            case TokenType::OP_LOGICAL_AND:
                node = std::shared_ptr<Node>(new LogicalOperationNode(LogicalOperator::AND, bufNodes[0], bufNodes[1]));
                break;
            case TokenType::OP_ARIPHMETIC_ADD:
                node = std::shared_ptr<Node>(new AriphmeticOperationNode(AriphmeticOperator::ADD, bufNodes[0], bufNodes[1]));
                break;
            case TokenType::OP_ARIPHMETIC_SUB:
                node = std::shared_ptr<Node>(new AriphmeticOperationNode(AriphmeticOperator::SUB, bufNodes[0], bufNodes[1]));
                break;
            case TokenType::OP_ARIPHMETIC_MUL:
                node = std::shared_ptr<Node>(new AriphmeticOperationNode(AriphmeticOperator::MUL, bufNodes[0], bufNodes[1]));
                break;
            case TokenType::OP_ARIPHMETIC_DIV:
                node = std::shared_ptr<Node>(new AriphmeticOperationNode(AriphmeticOperator::DIV, bufNodes[0], bufNodes[1]));
                break;
            case TokenType::OP_COMPARISON_MORE:
                node = std::shared_ptr<Node>(new ComparisonOperationNode(ComparisonOperator::MORE, bufNodes[0], bufNodes[1]));
                break;
            case TokenType::OP_COMPARISON_LESS:
                node = std::shared_ptr<Node>(new ComparisonOperationNode(ComparisonOperator::LESS, bufNodes[0], bufNodes[1]));
                break;
            case TokenType::OP_COMPARISON_EQUAL:
                node = std::shared_ptr<Node>(new ComparisonOperationNode(ComparisonOperator::EQUAL, bufNodes[0], bufNodes[1]));
                break;
            case TokenType::OP_COMPARISON_NOT_EQUAL:
                node = std::shared_ptr<Node>(new ComparisonOperationNode(ComparisonOperator::NOT_EQUAL, bufNodes[0], bufNodes[1]));
                break;
            case TokenType::OP_COMPARISON_MORE_EQUAL:
                node = std::shared_ptr<Node>(new ComparisonOperationNode(ComparisonOperator::MORE_EQUAL, bufNodes[0], bufNodes[1]));
                break;
            case TokenType::OP_COMPARISON_LESS_EQUAL:
                node = std::shared_ptr<Node>(new ComparisonOperationNode(ComparisonOperator::LESS_EQUAL, bufNodes[0], bufNodes[1]));
                break;
            default:
                throw std::runtime_error("bad operator type");
        }

        node->setIndex(std::max(bufNodes[0]->getIndex(), bufNodes[1]->getIndex()));

        nodes.push(node);
    }

    if(nodes.size() != 1) {
        throw std::runtime_error("bad expression");
    }
}

NodeValue Expression::eval(std::map<std::string, NodeValue> vars) {
    if(!nodes.size()) {
        throw std::runtime_error("need parsing before");
    }

    return nodes.top()->eval(vars);
}

std::string Expression::getInput() const {
    return input;
}

const std::vector<Token>& Expression::getTokens() const {
    return tokens;
}

const std::vector<Token>& Expression::getOperands() const {
    return operands;
}

const std::vector<Token>& Expression::getOperators() const {
    return operators;
}

std::ostream& operator <<(std::ostream& stream, const Expression& expr) {
    stream << expr.getInput() << std::endl;

    stream << "TOKENS: {" << std::endl;
    for(auto& item : expr.getTokens()) {
        stream << '\t' << item << std::endl;
    }
    stream << "}" << std::endl;

    stream << "OPERANDS: {" << std::endl;
    for(auto& item : expr.getOperands()) {
        stream << '\t' << item << std::endl;
    }
    stream << "}" << std::endl;

    stream << "OPERATORS: {" << std::endl;
    for(auto& item : expr.getOperators()) {
        stream << '\t' << item << std::endl;
    }
    return stream << "}" << std::endl;
}
