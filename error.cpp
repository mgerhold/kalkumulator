//
// Created by micha on 06.11.2022.
//

#include "error.hpp"
#include "tokens.hpp"
#include "utils.hpp"
#include <cassert>
#include <functional>
#include <iostream>

void print_error(const std::string_view input, const Token& token, const std::string_view error_message) {
    print_error(input, token.lexeme, error_message);
}

void print_error(const std::string_view input, const char* const position, const std::string_view error_message) {
    assert(std::greater_equal{}(position, input.data()) and std::less{}(position, input.data() + input.length()));
    const auto lexeme = std::string_view{ position, position + 1 };
    print_error(input, lexeme, error_message);
}

void print_error(const std::string_view input, const std::string_view lexeme, const std::string_view error_message) {
    if (lexeme.empty()) {
        std::cerr << "  error\n  reason: " << error_message << "\n";
        return;
    }
    const auto [begin, end] = lexeme_offsets(lexeme, input);
    for (usize i = 0; i < begin + 2; ++i) {
        std::cerr << " ";
    }
    std::cerr << "^";
    for (usize i = 0; i < lexeme.length() - 1; ++i) {
        std::cerr << "~";
    }
    std::cerr << " error occurred here\n";
    for (usize i = 0; i < begin + 2 + lexeme.length(); ++i) {
        std::cerr << " ";
    }
    std::cerr << " reason: " << error_message << "\n";
}
