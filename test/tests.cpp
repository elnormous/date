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

TEST_CASE("February 2, 2023", "[parsing]")
{
    const auto result = date::parse("2023-02-02");
    REQUIRE(result.year == 2023);
    REQUIRE(result.month == 2);
    REQUIRE(result.day == 2);
}

TEST_CASE("February 29 non-leap year", "[parsing]")
{
    const std::string s = "2023-02-29";
    REQUIRE_THROWS_AS(date::parse(s), date::ParseError);
}

TEST_CASE("April 31", "[parsing]")
{
    const std::string s = "2023-04-31";
    REQUIRE_THROWS_AS(date::parse(s), date::ParseError);
}

TEST_CASE("Parse number", "[parsing]")
{
    const std::string s = "2023";
    const auto [i, result] = date::parseNumbers(s.begin(), s.end(), 4);
    REQUIRE(i == s.end());
    REQUIRE(result == 2023);
}

TEST_CASE("Parse time with separators", "[parsing]")
{
    const std::string s = "2023-02-02T10:11:12";
    const auto result = date::parse(s);
    REQUIRE(result.year == 2023);
    REQUIRE(result.month == 2);
    REQUIRE(result.day == 2);
    REQUIRE(result.hour == 10);
    REQUIRE(result.minute == 11);
    REQUIRE(result.second == 12);
}

TEST_CASE("Invalid month", "[parsing]")
{
    const std::string s = "2023-13-02T10:11:12";
    REQUIRE_THROWS_AS(date::parse(s), date::ParseError);
}

TEST_CASE("Parse time without separators", "[parsing]")
{
    const std::string s = "20230202T101112";
    const auto result = date::parse(s);
    REQUIRE(result.year == 2023);
    REQUIRE(result.month == 2);
    REQUIRE(result.day == 2);
    REQUIRE(result.hour == 10);
    REQUIRE(result.minute == 11);
    REQUIRE(result.second == 12);
}
