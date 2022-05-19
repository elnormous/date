#include <cstddef>
#include "catch2/catch.hpp"
#include "date.hpp"

TEST_CASE("January 1, 2022", "[parsing]")
{
    const auto result = date::parse("2022-01-01");
    REQUIRE(result.year == 2022);
    REQUIRE(result.month == 1);
    REQUIRE(result.day == 1);
}

TEST_CASE("Parse year", "[parsing]")
{
    const std::string s = "2023";
    const auto [i, result] = date::parseYear(s.begin(), s.end());
    REQUIRE(i == s.end());
    REQUIRE(result == 2023);
}
