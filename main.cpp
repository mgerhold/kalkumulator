#include "parser.hpp"
#include "scanner.hpp"
#include <iostream>
#include <string>

[[nodiscard]] std::string read_input() {
    auto input = std::string{};
    std::cout << "> ";
    std::getline(std::cin, input);
    return input;
}

int main() {
    std::cout << "Kalkumulator\n"
                 "~~~~~~~~~~~~\n";
    // REPL - read evaluate print loop
    while (true) {
        if (not std::cin.good()) {
            break;
        }
        const auto input = read_input();
        if (input == "exit") {
            break;
        }
        /* evaluate input:
         * 1. tokenize input
         *    example: "(1 + 2) * 3"
         *    =>
         *    LeftParenthesis, IntegerLiteral, Plus, IntegerLiteral, RightParenthesis, Asterisk, IntegerLiteral
         */
        auto tokens = tokenize(input);

        if (not tokens.has_value()) {
            continue;
        }

        /* 2. parse tokens (result: abstract syntax tree, AST)
         *    BinaryOperator(BinaryOperator(IntegerValue, IntegerValue), IntegerValue)
         *
         *                                      BinaryOperator
         *                                  (OperatorType::Multiply)
         *                                      /              \
         *                                     /                \
         *                              BinaryOperator      IntegerValue
         *                         (OperatorType::Multiply)      (3)
         *                              /             \
         *                             /               \
         *                       IntegerValue     IntegerValue
         *                           (1)               (2)
         */

        auto parser = Parser{ input, std::move(*tokens) };

        try {
            const auto abstract_syntax_tree = parser.parse();

            /*3. evaluate AST
             *    recursively traverse the tree structure and evaluate the value of each tree node
             */
            const auto result = abstract_syntax_tree->evaluate();
            std::cout << result << "\n";
        } catch (const ParserError& exception) {
            print_error(exception.input, *(exception.token), exception.error_message);
        }
    }
}
