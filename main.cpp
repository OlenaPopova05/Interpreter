#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <sstream>
#include <cctype>
#include <iomanip>

using namespace std;

class Calculator {
    string _input;

public:
    Calculator(const string& input) : _input(input) {}

    vector<string> Tokenization() {
        vector<char> token;
        vector<string> tokens;
        vector<char> mathSymbols = { '+', '-', '*', '/', '(', ')' };
        vector<char> mathSymbolsWithoutMinus = { '+', '*', '/', '(', ')' };

        for (const auto& character : _input) {
            bool ifCharIsMinus = token.empty() && character != ' ' &&
                                 find(mathSymbolsWithoutMinus.begin(), mathSymbolsWithoutMinus.end(), character) == mathSymbolsWithoutMinus.end();
            if (isdigit(character) || character == '.') {
                token.push_back(character);
            }
            else if (ifCharIsMinus) {
                token.push_back(character);
            }
            else if (find(mathSymbols.begin(), mathSymbols.end(), character) != mathSymbols.end()) {
                if (!token.empty() && find(token.begin(), token.end(), '(') == token.end() &&
                    find(token.begin(), token.end(), ')') == token.end()) {
                    tokens.push_back(string(token.begin(), token.end()));
                    token.clear();
                }
                tokens.push_back(string(1, character));
            }
        }
        if (!token.empty()) {
            tokens.push_back(string(token.begin(), token.end()));
        }
        return tokens;
    }

    queue<string> ShuntingYard(const vector<string>& tokenList) {
        stack<string> stack;
        queue<string> output;
        unordered_map<string, int> priorities = {
            {"+", 1},
            {"-", 1},
            {"*", 2},
            {"/", 2}
        };

        for (const auto& token : tokenList) {
            float number;
            if (istringstream(token) >> number) {
                output.push(token);
            } else if (!isdigit(token[0]) && token != "(" && token != ")") {
                while (!stack.empty() && priorities[stack.top()] >= priorities[token] && stack.top() != "(") {
                    output.push(stack.top());
                    stack.pop();
                }
                stack.push(token);
            } else if (token == "(") {
                stack.push(token);
            } else if (token == ")") {
                while (!stack.empty() && stack.top() != "(") {
                    output.push(stack.top());
                    stack.pop();
                }
                stack.pop();
            }
        }

        while (!stack.empty()) {
            output.push(stack.top());
            stack.pop();
        }

        return output;
    }

    string Counting(queue<string>& output) {
        stack<string> countingStack;
        vector<string> mathSymbols = {"+", "-", "*", "/"};

        while (!output.empty()) {
            string token = output.front();
            output.pop();

            if (find(mathSymbols.begin(), mathSymbols.end(), token) == mathSymbols.end()) {
                countingStack.push(token);
            } else {
                double secondDigit = stod(countingStack.top());
                countingStack.pop();
                double firstDigit = stod(countingStack.top());
                countingStack.pop();

                double result;
                if (token == "+") {
                    result = firstDigit + secondDigit;
                } else if (token == "-") {
                    result = firstDigit - secondDigit;
                } else if (token == "*") {
                    result = firstDigit * secondDigit;
                } else if (token == "/") {
                    if (secondDigit != 0) {
                        result = firstDigit / secondDigit;
                    } else {
                        cout << "Error: Division by zero" << endl;
                        return "";
                    }
                }

                // Convert result to string with 6 decimal places
                stringstream ss;
                ss << fixed << setprecision(6) << result;
                countingStack.push(ss.str());
            }
        }

        return countingStack.top();
    }
};

int main() {
    string input;
    cout << "> ";
    getline(cin, input);

    Calculator calculator(input);

    vector<string> tokens = calculator.Tokenization();
    queue<string> postfix = calculator.ShuntingYard(tokens);
    string result = calculator.Counting(postfix);

    if (!result.empty()) {
        cout << "> " << result << endl;
    }

    return 0;
}


