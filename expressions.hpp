//
// Created by micha on 06.11.2022.
//

#pragma once

#include "types.hpp"
#include "error.hpp"
#include <cassert>
#include <memory>
#include <string>

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
    [[nodiscard]] virtual i64 evaluate() const = 0;
};

struct IntegerValue final : public Expression {
private:
    u32 m_value;

public:
    explicit IntegerValue(u32 value) : m_value{ value } { }

    [[nodiscard]] std::string to_string() const override {
        return std::to_string(m_value);
    }

    [[nodiscard]] i64 evaluate() const override {
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
        auto result = "(" + m_lhs->to_string();
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

    [[nodiscard]] i64 evaluate() const override {
        using namespace std::string_view_literals;
        const auto left = m_lhs->evaluate();
        const auto right = m_rhs->evaluate();
        switch (m_operator_type) {
            case BinaryOperatorType::Add:
                return left + right;
            case BinaryOperatorType::Subtract:
                return left - right;
            case BinaryOperatorType::Multiply:
                return left * right;
            case BinaryOperatorType::Divide:
                if (right == 0) {
                    print_error(""sv, ""sv, "divide by zero error"sv);
                    std::exit(1);
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
    explicit UnaryOperator(UnaryOperatorType operator_type, std::unique_ptr<Expression> sub_expression)
        : m_operator_type{ operator_type },
          m_sub_expression{ std::move(sub_expression) } { }

    [[nodiscard]] std::string to_string() const override {
        auto result = "(" + [&]() -> std::string {
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
        result += m_sub_expression->to_string() + ")";
        return result;
    }

    [[nodiscard]] i64 evaluate() const override {
        const auto sub_expression_value = m_sub_expression->evaluate();
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
