//
// Created by micha on 06.11.2022.
//

#pragma once

#include <string_view>

struct Token;

void print_error(std::string_view input, const Token& token, std::string_view error_message);
void print_error(std::string_view input, const char* position, std::string_view error_message);
void print_error(std::string_view input, std::string_view lexeme, std::string_view error_message);
