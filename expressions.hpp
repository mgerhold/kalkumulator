//
// Created by micha on 06.11.2022.
//

#pragma once

#include "error.hpp"
#include "types.hpp"
#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

struct EvaluationError final : public std::exception {
    std::string error_message;

    explicit EvaluationError(std::string error_message) : error_message{ std::move(error_message) } { }
};

using SymbolTable = std::unordered_map<std::string, i64>;

enum class BinaryOperatorType {
    Add,
    Subtract,
    Multiply,
    Divide,
};

enum class UnaryOperatorType {
    Plus,
    Minus,
};

struct Expression {
public:
    virtual ~Expression() = default;
    [[nodiscard]] virtual std::string to_string() const = 0;
    [[nodiscard]] virtual i64 evaluate(SymbolTable&) const = 0;
};

struct IntegerValue final : public Expression {
private:
    u32 m_value;

public:
    explicit IntegerValue(u32 value) : m_value{ value } { }

    [[nodiscard]] std::string to_string() const override {
        return std::to_string(m_value);
    }

    [[nodiscard]] i64 evaluate(SymbolTable&) const override {
        return static_cast<i64>(m_value);
    }
};

struct BinaryOperator final : public Expression {
private:
    std::unique_ptr<Expression> m_lhs;
    BinaryOperatorType m_operator_type;
    std::unique_ptr<Expression> m_rhs;

public:
    BinaryOperator(std::unique_ptr<Expression> lhs, BinaryOperatorType operator_type, std::unique_ptr<Expression> rhs)
        : m_lhs{ std::move(lhs) },
          m_operator_type{ operator_type },
          m_rhs{ std::move(rhs) } { }

    [[nodiscard]] std::string to_string() const override {
        using namespace std::string_literals;

        auto result = "("s;
        result += m_lhs->to_string();
        switch (m_operator_type) {
            case BinaryOperatorType::Add:
                result += " + ";
                break;
            case BinaryOperatorType::Subtract:
                result += " - ";
                break;
            case BinaryOperatorType::Multiply:
                result += " * ";
                break;
            case BinaryOperatorType::Divide:
                result += " / ";
                break;
            default:
                assert(false and "unreachable");
                break;
        }
        result += m_rhs->to_string() + ")";
        return result;
    }

    [[nodiscard]] i64 evaluate(SymbolTable& symbol_table) const override {
        using namespace std::string_view_literals;
        const auto left = m_lhs->evaluate(symbol_table);
        const auto right = m_rhs->evaluate(symbol_table);
        switch (m_operator_type) {
            case BinaryOperatorType::Add:
                return left + right;
            case BinaryOperatorType::Subtract:
                return left - right;
            case BinaryOperatorType::Multiply:
                return left * right;
            case BinaryOperatorType::Divide:
                if (right == 0) {
                    throw EvaluationError{ "divide by zero error" };
                }
                return left / right;
            default:
                assert(false and "unreachable");
                return 0;
        }
    }
};

struct UnaryOperator final : public Expression {
private:
    UnaryOperatorType m_operator_type;
    std::unique_ptr<Expression> m_sub_expression;

public:
    UnaryOperator(UnaryOperatorType operator_type, std::unique_ptr<Expression> sub_expression)
        : m_operator_type{ operator_type },
          m_sub_expression{ std::move(sub_expression) } { }

    [[nodiscard]] std::string to_string() const override {
        using namespace std::string_literals;
        const auto operator_text = [&]() -> std::string {
            switch (m_operator_type) {
                case UnaryOperatorType::Plus:
                    return "+";
                case UnaryOperatorType::Minus:
                    return "-";
                default:
                    assert(false and "unreachable");
                    return "";
            }
        }();

        auto result = "("s + operator_text;
        result += m_sub_expression->to_string() + ")";
        return result;
    }

    [[nodiscard]] i64 evaluate(SymbolTable& symbol_table) const override {
        const auto sub_expression_value = m_sub_expression->evaluate(symbol_table);
        switch (m_operator_type) {
            case UnaryOperatorType::Plus:
                return sub_expression_value;
            case UnaryOperatorType::Minus:
                return -sub_expression_value;
            default:
                assert(false and "unreachable");
                return 0;
        }
    }
};

struct Assignment final : public Expression {
private:
    std::string_view m_variable_name;
    std::unique_ptr<Expression> m_value;

public:
    Assignment(std::string_view variable_name, std::unique_ptr<Expression> value)
        : m_variable_name{ variable_name },
          m_value{ std::move(value) } { }

    [[nodiscard]] std::string to_string() const override {
        return std::string{ m_variable_name };
    }

    [[nodiscard]] i64 evaluate(SymbolTable& symbol_table) const override {
        const auto value = m_value->evaluate(symbol_table);
        symbol_table[std::string{ m_variable_name }] = value;
        return value;
    }
};

struct Variable final : public Expression {
private:
    std::string_view m_variable_name;

public:
    explicit Variable(std::string_view variable_name) : m_variable_name{ variable_name } { }

    [[nodiscard]] std::string to_string() const override {
        return std::string{ m_variable_name };
    }

    [[nodiscard]] i64 evaluate(SymbolTable& symbol_table) const override {
        using namespace std::string_literals;

        const auto find_iterator = symbol_table.find(std::string{ m_variable_name });
        const auto found = (find_iterator != symbol_table.cend());
        if (not found) {
            throw EvaluationError{ "use of undefined variable \""s + std::string{ m_variable_name } + "\"" };
        }
        return find_iterator->second;
    }
};
