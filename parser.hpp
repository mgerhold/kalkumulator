//
// Created by micha on 06.11.2022.
//

#pragma once

#include "expressions.hpp"
#include "scanner.hpp"
#include "tokens.hpp"
#include <stdexcept>

struct ParserError : public std::exception {
    ParserError(std::string_view input, const Token* token, std::string_view error_message)
        : input{ input },
          token{ token },
          error_message{ error_message } { }

    std::string_view input;
    const Token* token;
    std::string_view error_message;
};

class Parser final {
private:
    std::string_view m_input;
    TokenList m_tokens;
    usize m_index{ 0 };

public:
    Parser(const std::string_view input, TokenList tokens) : m_input{ input }, m_tokens{ std::move(tokens) } { }

    [[nodiscard]] std::unique_ptr<Expression> parse() {
        m_index = 0;
        return expression();
    }

private:
    [[nodiscard]] std::unique_ptr<Expression> expression() {
        return addition_or_subtraction();
    }

    [[nodiscard]] std::unique_ptr<Expression> addition_or_subtraction() {
        auto accumulator = multiplication_or_division();
        while (current().is_plus() or current().is_minus()) {
            const auto operator_type = (current().is_plus() ? BinaryOperatorType::Add : BinaryOperatorType::Subtract);
            advance();
            auto rhs = multiplication_or_division();
            accumulator = std::make_unique<BinaryOperator>(std::move(accumulator), operator_type, std::move(rhs));
        }
        return accumulator;
    }

    [[nodiscard]] std::unique_ptr<Expression> multiplication_or_division() {
        auto accumulator = unary_plus_or_minus();
        while (current().is_asterisk() or current().is_forward_slash()) {
            const auto operator_type =
                    (current().is_asterisk() ? BinaryOperatorType::Multiply : BinaryOperatorType::Divide);
            advance();
            auto rhs = unary_plus_or_minus();
            accumulator = std::make_unique<BinaryOperator>(std::move(accumulator), operator_type, std::move(rhs));
        }
        return accumulator;
    }

    [[nodiscard]] std::unique_ptr<Expression> unary_plus_or_minus() {
        if (current().is_plus() or current().is_minus()) {
            const auto operator_type = (current().is_plus() ? UnaryOperatorType::Plus : UnaryOperatorType::Minus);
            advance();
            return std::make_unique<UnaryOperator>(operator_type, unary_plus_or_minus());
        }
        return primary();
    }

    [[nodiscard]] std::unique_ptr<Expression> primary() {
        if (current().is_integer_literal()) {
            auto result = std::make_unique<IntegerValue>(current().value());
            advance();
            return result;
        }
        if (current().is_left_parenthesis()) {
            // 1 * (3 + 4) * (2) * ((3))
            advance();
            auto inner_expression = expression();
            if (not current().is_right_parenthesis()) {
                throw ParserError{ m_input, &current(), "expected \")\"" };
            }
            advance();
            return inner_expression;
        }
        if (current().is_end_of_input()) {
            throw ParserError{ m_input, &current(), "unexpected end of input" };
        }
        throw ParserError{ m_input, &current(), "unexpected token" };
    }

    [[nodiscard]] const Token& current() const {
        return *m_tokens.at(m_index);
    }

    [[nodiscard]] const Token& next() const {
        return *m_tokens.at(m_index + 1);
    }

    void advance() {
        ++m_index;
    }
};
