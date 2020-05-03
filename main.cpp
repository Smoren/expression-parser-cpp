#include <iostream>
#include <vector>
#include "mysharedpointer.h"
#include "node.h"
#include "parser.h"

using namespace std;

void testMySharedPointer() {
        vector<MySharedPointer<int>> v;
        for(int i=0; i<3; i++) {
            v.push_back(MySharedPointer<int>(new int(i)));
        }

        cout << *(v[0].get()) << endl;

        MySharedPointer<int> p11 = v[1];
        MySharedPointer<int> p12 = v[1];

        cout << "finish" << endl;
}

void testConstExpression() {
    double x = 1.0;
    double y = 20.0;

    //cin >> x >> y;
    std::map<std::string, NodeValue> vars = {{"x", NodeValue(x)}, {"y", NodeValue(y)}};

    shared_ptr<Node> a = make_shared<ConstNode>(NodeValue(10.0));
    shared_ptr<Node> b = make_shared<VarNode>("x");
    shared_ptr<Node> ab = make_shared<AriphmeticOperationNode>(AriphmeticOperator::SUB, a, b);

    shared_ptr<Node> abCmp = make_shared<ComparisonOperationNode>(ComparisonOperator::MORE, ab, b);
    cout << "10 - " << x << " > " << x << " = " << abCmp->eval(vars) << endl;

    shared_ptr<Node> c = make_shared<VarNode>("y");
    shared_ptr<Node> d = make_shared<ConstNode>(NodeValue(50.0));

    shared_ptr<Node> cdCmp = make_shared<ComparisonOperationNode>(ComparisonOperator::LESS, c, d);
    cout  << y << " < 50 = " << cdCmp->eval(vars) << endl;

    shared_ptr<Node> abAndCd = make_shared<LogicalOperationNode>(LogicalOperator::AND, abCmp, cdCmp);
    cout << "(10 - " << x << " > " << x << ") AND (" << y << " < 50) = " << abAndCd->eval(vars) << endl;

    // стековая арифметика?
}

int main()
{
    //testMySharedPointer();
    //testConstExpression();

    //Expression expr("((1+2)*3+4*5)*5+6*(7-8)");
    Expression expr("((1+x)*y+4*5)*y+6*(x-y) >= 157+1");
    //Expression expr("1 + 3 * 2 = 157");
    //Expression expr("(x+y)*y + 10*y + (22+y)");
    //Expression expr("x*y+6*(3-2)-1");
    //Expression expr("7*3+6*3-1");
    expr.parse();

    cout << expr.eval({{"x", NodeValue(2.0)}, {"y", NodeValue(5.0)}}) << endl;
    //cout << expr << endl;

    return 0;
}
