#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <cctype>

using namespace std;

class Calculator {
    string _input;

public:
Calculator(const string& input) : _input(input) {}

vector<string> Tokenization(const string& input) {
    vector<char> functionToken;
    vector<char> token;
    vector<string> tokens;
    vector<char> mathSymbols = {'+', '-', '*', '/', '(', ')', ','};
    vector<char> mathSymbolsWithoutMinus = {'+', '*', '/', '(', ')', ','};
    vector<string> functions = {"pow", "abs", "min", "max"};

    for (char character : input) {
        bool ifCharIsMinus = token.empty() && character != ' '
                            && find(mathSymbolsWithoutMinus.begin(), mathSymbolsWithoutMinus.end(), character) == mathSymbolsWithoutMinus.end()
                            && !isalpha(character);

        if (isdigit(character) || character == '.') {
            token.push_back(character);
        } else if (ifCharIsMinus) {
            token.push_back(character);
        } else if (isalpha(character)) {
            functionToken.push_back(character);
        } else if (character == ',') {
            if (!token.empty()) {
                tokens.push_back(string(token.begin(), token.end()));
                token.clear();
            }
            tokens.push_back(",");
        }else if (find(mathSymbols.begin(), mathSymbols.end(), character) != mathSymbols.end()) {
            if (!functionToken.empty()) {
                string functionSymbol(functionToken.begin(), functionToken.end());
                if (find(functions.begin(), functions.end(), functionSymbol) != functions.end()) {
                    tokens.push_back(functionSymbol);
                    functionToken.clear();
                } else {
                    cout << "Input pow, abs, min, or max correctly" << endl;
                    exit(0);
                }
            }
            if (!token.empty()) {
                tokens.push_back(string(token.begin(), token.end()));
                token.clear();
            }
            tokens.push_back(string(1, character));
        }
    }
    if (!token.empty()) {
        tokens.push_back(string(token.begin(), token.end()));
    }
    if (!functionToken.empty()) {
        string functionSymbol(functionToken.begin(), functionToken.end());
        if (find(functions.begin(), functions.end(), functionSymbol) != functions.end()) {
            tokens.push_back(functionSymbol);
        } else {
            cout << "Input pow, abs, min, or max correctly" << endl;
            exit(0);
        }
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
        {"/", 2},
        {"pow", 2},
        {"abs", 2},
        {"min", 2},
        {"max", 2}
    };

    for (const string& token : tokenList) {
        char* end;
        float number = strtof(token.c_str(), &end);
        if (end != token.c_str() && *end == '\0') {
            output.push(token);
        }else if (token == ",") {
            // Ensure all arguments within the function are processed
            while (!stack.empty() && stack.top() != "(") {
                output.push(stack.top());
                stack.pop();
            }
        }else if (end == token.c_str() && token != "(" && token != ")") {
            while (!stack.empty() && priorities[stack.top()] >= priorities[token] && stack.top() != "(") {
                output.push(stack.top());
                stack.pop();
            }
            stack.push(token);
        }

        if (token == "(") {
            stack.push(token);
        } else if (token == ")") {
            while (stack.top() != "(") {
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
    vector<string> mathSymbols = {"+", "-", "*", "/", "pow", "abs", "min", "max"};

    while (!output.empty()) {
        string token = output.front();
        output.pop();

        if (find(mathSymbols.begin(), mathSymbols.end(), token) == mathSymbols.end()) {
            countingStack.push(token);
        } else {
            if (token == "pow" || token == "abs" || token == "min" || token == "max") {
                string arg1, arg2;
                if (token == "pow" || token == "min" || token == "max") {
                    arg2 = countingStack.top();
                    countingStack.pop();
                    arg1 = countingStack.top();
                    countingStack.pop();
                } else {
                    arg1 = countingStack.top();
                    countingStack.pop();
                }
                if (token == "pow") {
                    countingStack.push(to_string(pow(stod(arg1), stod(arg2))));
                } else if (token == "abs") {
                    countingStack.push(to_string(abs(stod(arg1))));
                } else if (token == "min") {
                    countingStack.push(to_string(min(stod(arg1), stod(arg2))));
                } else if (token == "max") {
                    countingStack.push(to_string(max(stod(arg1), stod(arg2))));
                }
            } else {
                double secondDigit = stod(countingStack.top());
                countingStack.pop();
                double firstDigit = stod(countingStack.top());
                countingStack.pop();
                if (token == "+") {
                    countingStack.push(to_string(firstDigit + secondDigit));
                } else if (token == "-") {
                    countingStack.push(to_string(firstDigit - secondDigit));
                } else if (token == "*") {
                    countingStack.push(to_string(firstDigit * secondDigit));
                } else if (token == "/") {
                    if (secondDigit!= 0) {
                        countingStack.push(to_string(firstDigit / secondDigit));
                    } else {
                        cout << "You cannot divide by zero" << endl;
                        exit(1);
                    }
                }
            }
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
    vector<string> tokens = calculator.Tokenization(input);
    queue<string> postfix = calculator.ShuntingYard(tokens);
    string result = calculator.Counting(postfix);
    if (!result.empty()) {
        cout << "> " << result << endl;
    }

    return 0;
}


