//
// Created by micha on 05.11.2022.
//

#pragma once

#include "types.hpp"
#include <cassert>
#include <functional>
#include <string_view>
#include <utility>

struct Token {
    std::string_view lexeme; // the part of the input string that corresponds to this token (if this is empty, it
                             // does not correspond to a part of the actual input)

    explicit Token(const std::string_view lexeme) : lexeme{ lexeme } { }
    virtual ~Token() = default;

    [[nodiscard]] virtual bool is_plus() const {
        return false;
    }

    [[nodiscard]] virtual bool is_minus() const {
        return false;
    }

    [[nodiscard]] virtual bool is_asterisk() const {
        return false;
    }

    [[nodiscard]] virtual bool is_forward_slash() const {
        return false;
    }

    [[nodiscard]] virtual bool is_equals() const {
        return false;
    }

    [[nodiscard]] virtual bool is_left_parenthesis() const {
        return false;
    }

    [[nodiscard]] virtual bool is_right_parenthesis() const {
        return false;
    }

    [[nodiscard]] virtual bool is_integer_literal() const {
        return false;
    }

    [[nodiscard]] virtual bool is_end_of_input() const {
        return false;
    }

    [[nodiscard]] virtual bool is_identifier() const {
        return false;
    }

    [[nodiscard]] virtual u32 value() const {
        assert(false and "unreachable");
        return 0;
    }
};

struct LeftParenthesis final : public Token {
    using Token::Token;

    [[nodiscard]] bool is_left_parenthesis() const override {
        return true;
    }
};

struct RightParenthesis final : public Token {
    using Token::Token;

    [[nodiscard]] bool is_right_parenthesis() const override {
        return true;
    }
};

struct Plus final : public Token {
    using Token::Token;

    [[nodiscard]] bool is_plus() const override {
        return true;
    }
};

struct Minus final : public Token {
    using Token::Token;

    [[nodiscard]] bool is_minus() const override {
        return true;
    }
};

struct Asterisk final : public Token {
    using Token::Token;

    [[nodiscard]] bool is_asterisk() const override {
        return true;
    }
};

struct ForwardSlash final : public Token {
    using Token::Token;

    [[nodiscard]] bool is_forward_slash() const override {
        return true;
    }
};

struct Equals final : public Token {
    using Token::Token;

    [[nodiscard]] bool is_equals() const override {
        return true;
    }
};

struct IntegerLiteral final : public Token {
private:
    u32 m_value;

public:
    IntegerLiteral(const std::string_view lexeme, const u32 value) : Token::Token{ lexeme }, m_value{ value } { }

    [[nodiscard]] u32 value() const override {
        return m_value;
    }

    [[nodiscard]] bool is_integer_literal() const override {
        return true;
    }
};

struct EndOfInput final : public Token {
    using Token::Token;

    [[nodiscard]] bool is_end_of_input() const override {
        return true;
    }
};

struct Identifier final : public Token {
    using Token::Token;

    [[nodiscard]] bool is_identifier() const override {
        return true;
    }
};
