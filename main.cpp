#include <iostream>
#include <vector>
#include <map>
#include "node.h"
#include "parser.h"
#include "testing.h"

using namespace std;

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
