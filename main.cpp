#include <iostream>
#include <vector>
#include <map>
#include "mysharedpointer.h"
#include "node.h"
#include "parser.h"
#include "testing.h"

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

string getExpressionFromInput(istream& stream) {
    string result;

    while(stream.peek() == ' ') {
        stream.ignore(1);
    }
    getline(stream, result);

    return result;
}

int main()
{
    //testConstExpression();
    //testExpressionParsing();

    cout << "Input expression: ";
    std::string input = getExpressionFromInput(cin);

    Expression expr(input);
    cout << "Polish notation: " << expr.getPolishNotation() << endl;

    const set<string>& variables = expr.getVariables();
    map<string, NodeValue> varValues;

    for(const string& varName : variables) {
        double varValue;
        cout << varName << ": ";
        cin >> varValue;
        varValues.insert({varName, NodeValue(varValue)});
    }

    cout << "result: " << expr.eval(varValues) << endl;

    return 0;
}
