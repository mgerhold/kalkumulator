//
// Created by micha on 06.11.2022.
//

#include "utils.hpp"
#include "tokens.hpp"
#include <functional>

[[nodiscard]] std::pair<usize, usize> lexeme_offsets(const std::string_view lexeme, const std::string_view input) {
    if (input.empty()) {
        return { 0, 0 };
    }

    /* assert that the data-pointer of the lexeme is inside the passed input.
         * We're using std::greater_equal and std::less here to avoid UB because of
         * "regular" pointer comparisons. */
    assert(std::greater_equal{}(lexeme.data(), input.data())
           and std::less{}(lexeme.data(), input.data() + input.length()));

    const auto begin = static_cast<usize>(lexeme.data() - input.data());
    const auto end = begin + lexeme.length();
    return { begin, end };
}
