//
// Created by micha on 06.11.2022.
//

#pragma once

#include <utility>
#include <string_view>
#include "types.hpp"

[[nodiscard]] std::pair<usize, usize> lexeme_offsets(std::string_view lexeme, std::string_view input);
