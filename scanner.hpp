//
// Created by micha on 05.11.2022.
//

#pragma once

#include "error.hpp"
#include "tokens.hpp"
#include <cctype>
#include <charconv>
#include <iostream>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

using TokenList = std::vector<std::unique_ptr<Token>>;

template<typename TokenType>
inline void add_token(TokenList& tokens, const char* const current_char, const usize length = 1) {
    tokens.emplace_back(std::make_unique<TokenType>(std::string_view{ current_char, current_char + length }));
}

[[nodiscard]] inline std::optional<TokenList> tokenize(const std::string_view input) {
    auto tokens = TokenList{};
    for (usize i = 0; i < input.size();) {
        const auto& current = input.at(i);
        usize token_length = 1;
        switch (current) {
            case '(':
                add_token<LeftParenthesis>(tokens, &current);
                break;
            case ')':
                add_token<RightParenthesis>(tokens, &current);
                break;
            case '+':
                add_token<Plus>(tokens, &current);
                break;
            case '-':
                add_token<Minus>(tokens, &current);
                break;
            case '*':
                add_token<Asterisk>(tokens, &current);
                break;
            case '/':
                add_token<ForwardSlash>(tokens, &current);
                break;
            default:
                if (std::isspace(current)) {
                    break;
                }
                if (std::isdigit(current)) {
                    const usize integer_start = i;
                    ++i;
                    while (i < input.length() and std::isdigit(input.at(i))) {
                        i += 1;
                        ++token_length;
                    }
                    const auto integer_view = std::string_view{ input.data() + integer_start,
                                                                input.data() + integer_start + token_length };
                    auto parsed_value = u32{};
                    const auto conversion_result = std::from_chars(
                            integer_view.data(), integer_view.data() + integer_view.length(), parsed_value
                    );
                    assert(conversion_result.ec != std::errc::invalid_argument);
                    const auto out_of_range = (conversion_result.ec == std::errc::result_out_of_range);

                    if (out_of_range) {
                        print_error(input, integer_view, "integer literal out of bounds");
                        return {};
                    }

                    tokens.emplace_back(std::make_unique<IntegerLiteral>(integer_view, parsed_value));
                    continue;
                }
                print_error(input, &current, "unexpected input");
                return {};
        }
        i += token_length;
    }
    tokens.emplace_back(std::make_unique<EndOfInput>(""));
    return tokens;
}
