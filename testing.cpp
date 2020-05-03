#include <memory>
#include <string>
#include "node.h"
#include "parser.h"
#include "testing.h"

using namespace std;

void testConstExpression() {
    std::cout << "===================" << std::endl;
    std::cout << "testConstExpression" << std::endl;
    std::cout << "===================" << std::endl;

    double x = 1.0;
    double y = 20.0;

    std::map<std::string, NodeValue> vars = {{"x", NodeValue(x)}, {"y", NodeValue(y)}};

    shared_ptr<Node> a = make_shared<ConstNode>(NodeValue(10.0));
    shared_ptr<Node> b = make_shared<VarNode>("x");
    shared_ptr<Node> ab = make_shared<AriphmeticOperationNode>(AriphmeticOperator::SUB, a, b);

    shared_ptr<Node> abCmp = make_shared<ComparisonOperationNode>(ComparisonOperator::MORE, ab, b);
    std::cout << "10 - " << x << " > " << x << " = " << abCmp->eval(vars) << std::endl;

    shared_ptr<Node> c = make_shared<VarNode>("y");
    shared_ptr<Node> d = make_shared<ConstNode>(NodeValue(50.0));

    shared_ptr<Node> cdCmp = make_shared<ComparisonOperationNode>(ComparisonOperator::LESS, c, d);
    std::cout << y << " < 50 = " << cdCmp->eval(vars) << std::endl;

    shared_ptr<Node> abAndCd = make_shared<LogicalOperationNode>(LogicalOperator::AND, abCmp, cdCmp);
    std::cout << "(10 - " << x << " > " << x << ") AND (" << y << " < 50) = " << abAndCd->eval(vars) << std::endl;

    std::cout << std::endl;
}

void testExpressionParsing() {
    std::cout << "=====================" << std::endl;
    std::cout << "testExpressionParsing" << std::endl;
    std::cout << "=====================" << std::endl;

    Expression expr("((1+x)*y+4*5)*y+6*(x-y) >= 157+1");
    std::cout << expr << std::endl;
    std::cout << "Result: " << expr.eval({{"x", NodeValue(2.0)}, {"y", NodeValue(5.0)}}) << std::endl;

    std::cout << std::endl;
}
