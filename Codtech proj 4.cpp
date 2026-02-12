// Simple Compiler Design Project
// Implements lexical analysis + parsing + evaluation
// Supports: +, -, *, /, parentheses, integers

#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

// ---------------- LEXER ----------------

class Lexer {
    string input;
    int pos;

public:
    Lexer(string text) : input(text), pos(0) {}

    char peek() {
        if (pos < input.length())
            return input[pos];
        return '\0';
    }

    char get() {
        if (pos < input.length())
            return input[pos++];
        return '\0';
    }
};

// ---------------- PARSER ----------------

class Parser {
    Lexer lexer;
    char current;

public:
    Parser(string text) : lexer(text) {
        current = lexer.get();
    }

    void error() {
        throw runtime_error("Invalid Expression!");
    }

    void eat(char expected) {
        if (current == expected)
            current = lexer.get();
        else
            error();
    }

    // Forward declarations
    int expr() {
        int result = term();

        while (current == '+' || current == '-') {
            if (current == '+') {
                eat('+');
                result += term();
            } else {
                eat('-');
                result -= term();
            }
        }
        return result;
    }

    int term() {
        int result = factor();

        while (current == '*' || current == '/') {
            if (current == '*') {
                eat('*');
                result *= factor();
            } else {
                eat('/');
                int divisor = factor();
                if (divisor == 0)
                    throw runtime_error("Division by zero!");
                result /= divisor;
            }
        }
        return result;
    }

    int factor() {
        int result = 0;

        if (isdigit(current)) {
            while (isdigit(current)) {
                result = result * 10 + (current - '0');
                current = lexer.get();
            }
        }
        else if (current == '(') {
            eat('(');
            result = expr();
            eat(')');
        }
        else {
            error();
        }
        return result;
    }
};

// ---------------- MAIN (Compiler Driver) ----------------

int main() {
    string input;

    cout << "Simple Arithmetic Compiler\n";
    cout << "Supports: +  -  *  /  ( )\n";
    cout << "Enter expression: ";

    getline(cin, input);

    try {
        Parser parser(input);
        int result = parser.expr();
        cout << "Result after parsing: " << result << endl;
    }
    catch (exception &e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}