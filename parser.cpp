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

Token::Token(std::string value) : value(value) {
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
    } else if(value == "&") {
        type = TokenType::OP_LOGICAL_AND;
    } else if(value == "|") {
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
}

std::string Token::getValue() const {
    return value;
}

TokenType Token::getType() const {
    return type;
}

int Token::getPriority() const {
    return static_cast<int>(Token::priorityMap.at(type));
}

double Token::getDoubleValue() const {
    return std::stod(value);
}

std::string Token::getStringValue() const {
    return value;
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
    return stream << "'" << item.getValue() << "' (" << item.getType() << ", " << item.getPriority() << ")";
}

Expression::Expression(std::string input) : input(input) {

}

void Expression::tokenize() {
    std::stringstream buf;
    std::deque<Token> _tokens;

    // сначала распарсим токены
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
            case '=':
            case '&':
            case '|': {
                if(buf.str().size()) {
                    _tokens.push_back(Token(buf.str()));
                    buf.str("");
                }

                buf << ch;
                _tokens.push_back(Token(buf.str()));
                buf.str("");
                break;
            }
            default: {
                buf << ch;
            }
        }
    }

    if(buf.str().size()) {
        _tokens.push_back(Token(buf.str()));
        buf.str("");
    }

    std::stack<Token> tokenStack;
    int nestedLevel = 0;

    // теперь приведем к польской обратной записи
    while(_tokens.size()) {
        Token& t = _tokens.front();
        TokenType type = t.getType();
        _tokens.pop_front();
        if(type == TokenType::BRACKET_OPEN) {
            ++nestedLevel;
            tokenStack.push(t);
        } else if(type == TokenType::BRACKET_CLOSE) {
            --nestedLevel;
            while(tokenStack.size()) {
                Token& stackTop = tokenStack.top();
                tokenStack.pop();
                if(stackTop.getType() == TokenType::BRACKET_OPEN) {
                    break;
                }

                tokens.push_back(stackTop);
            }
        } else if(type == TokenType::NUMBER || type == TokenType::VARIABLE) {
            tokens.push_back(t);
        } else {
            if(!tokenStack.size()) {
                tokenStack.push(t);
                continue;
            }

            Token& stackTop = tokenStack.top();
            if(stackTop.getType() == TokenType::BRACKET_OPEN) {
                tokenStack.push(t);
            } else {
                while(stackTop.getType() != TokenType::BRACKET_OPEN && t.getPriority() < stackTop.getPriority()) {
                    tokens.push_back(stackTop);
                    tokenStack.pop();

                    if(!tokenStack.size()) {
                        break;
                    }
                    stackTop = tokenStack.top();
                }
                tokenStack.push(t);
            }
        }
    }

    while(tokenStack.size()) {
        Token& stackTop = tokenStack.top();
        tokenStack.pop();
        if(stackTop.getType() != TokenType::BRACKET_OPEN) {
            tokens.push_back(stackTop);
        }
    }

    if(nestedLevel != 0) {
        std::cout << nestedLevel << std::endl;
        throw std::runtime_error("nested level error: ");
    }
}

void Expression::parse() {
    tokenize();

    std::cout << input << std::endl;
    for(auto& token : tokens) {
        std::cout << token.getStringValue() << " ";
    }
    std::cout << std::endl;

    std::stack< std::shared_ptr<Node> > operandStack;
    std::stack<TokenType> operatorStack;

    while(tokens.size()) {
        Token& t = tokens.front();
        tokens.pop_front();
        TokenType type = t.getType();
        std::shared_ptr<Node> node;

        if(type == TokenType::NUMBER) {
            operandStack.push(std::shared_ptr<ConstNode>(new ConstNode(NodeValue(t.getDoubleValue()))));
        } else if(type == TokenType::VARIABLE) {
            operandStack.push(std::shared_ptr<VarNode>(new VarNode(t.getStringValue())));
        } else {
            operatorStack.push(type);

            while(operatorStack.size() > 0 && operandStack.size() > 1) {
                type = operatorStack.top();
                operatorStack.pop();

                auto rhsOperand = operandStack.top();
                operandStack.pop();
                auto lhsOperand = operandStack.top();
                operandStack.pop();

                switch(type) {
                    case TokenType::OP_LOGICAL_OR:
                        node = std::shared_ptr<Node>(new LogicalOperationNode(LogicalOperator::OR, lhsOperand, rhsOperand));
                        break;
                    case TokenType::OP_LOGICAL_AND:
                        node = std::shared_ptr<Node>(new LogicalOperationNode(LogicalOperator::AND, lhsOperand, rhsOperand));
                        break;
                    case TokenType::OP_ARIPHMETIC_ADD:
                        node = std::shared_ptr<Node>(new AriphmeticOperationNode(AriphmeticOperator::ADD, lhsOperand, rhsOperand));
                        break;
                    case TokenType::OP_ARIPHMETIC_SUB:
                        node = std::shared_ptr<Node>(new AriphmeticOperationNode(AriphmeticOperator::SUB, lhsOperand, rhsOperand));
                        break;
                    case TokenType::OP_ARIPHMETIC_MUL:
                        node = std::shared_ptr<Node>(new AriphmeticOperationNode(AriphmeticOperator::MUL, lhsOperand, rhsOperand));
                        break;
                    case TokenType::OP_ARIPHMETIC_DIV:
                        node = std::shared_ptr<Node>(new AriphmeticOperationNode(AriphmeticOperator::DIV, lhsOperand, rhsOperand));
                        break;
                    case TokenType::OP_COMPARISON_MORE:
                        node = std::shared_ptr<Node>(new ComparisonOperationNode(ComparisonOperator::MORE, lhsOperand, rhsOperand));
                        break;
                    case TokenType::OP_COMPARISON_LESS:
                        node = std::shared_ptr<Node>(new ComparisonOperationNode(ComparisonOperator::LESS, lhsOperand, rhsOperand));
                        break;
                    case TokenType::OP_COMPARISON_EQUAL:
                        node = std::shared_ptr<Node>(new ComparisonOperationNode(ComparisonOperator::EQUAL, lhsOperand, rhsOperand));
                        break;
                    case TokenType::OP_COMPARISON_NOT_EQUAL:
                        node = std::shared_ptr<Node>(new ComparisonOperationNode(ComparisonOperator::NOT_EQUAL, lhsOperand, rhsOperand));
                        break;
                    case TokenType::OP_COMPARISON_MORE_EQUAL:
                        node = std::shared_ptr<Node>(new ComparisonOperationNode(ComparisonOperator::MORE_EQUAL, lhsOperand, rhsOperand));
                        break;
                    case TokenType::OP_COMPARISON_LESS_EQUAL:
                        node = std::shared_ptr<Node>(new ComparisonOperationNode(ComparisonOperator::LESS_EQUAL, lhsOperand, rhsOperand));
                        break;
                    default:
                        throw std::runtime_error("bad operator type");
                }

                operandStack.push(node);
            }
        }
    }

    if(operandStack.size() != 1) {
        throw std::runtime_error("smth went wrong");
    }

    nodes.push(operandStack.top());
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

const std::deque<Token>& Expression::getTokens() const {
    return tokens;
}

std::ostream& operator <<(std::ostream& stream, const Expression& expr) {
    stream << expr.getInput() << std::endl;

    stream << "TOKENS: {" << std::endl;
    for(auto& item : expr.getTokens()) {
        stream << '\t' << item << std::endl;
    }

    return stream << "}" << std::endl;
}
