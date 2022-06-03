#include <cstddef>
#include "catch2/catch.hpp"
#include "date.hpp"

TEST_CASE("Leap year", "[parsing]")
{
    REQUIRE(date::isLeapYear(2000));
    REQUIRE(date::isLeapYear(2020));
    REQUIRE_FALSE(date::isLeapYear(2023));
    REQUIRE(date::isLeapYear(2024));
    REQUIRE_FALSE(date::isLeapYear(2100));
}

TEST_CASE("30-day month", "[parsing]")
{
    REQUIRE_FALSE(date::is30DayMonth(1));
    REQUIRE_FALSE(date::is30DayMonth(2));
    REQUIRE_FALSE(date::is30DayMonth(3));
    REQUIRE(date::is30DayMonth(4));
    REQUIRE_FALSE(date::is30DayMonth(5));
    REQUIRE(date::is30DayMonth(6));
    REQUIRE_FALSE(date::is30DayMonth(7));
    REQUIRE_FALSE(date::is30DayMonth(8));
    REQUIRE(date::is30DayMonth(9));
    REQUIRE_FALSE(date::is30DayMonth(10));
    REQUIRE(date::is30DayMonth(11));
    REQUIRE_FALSE(date::is30DayMonth(12));
}

TEST_CASE("Days in month", "[parsing]")
{
    REQUIRE(date::getDaysInMonth(1, 2022) == 31);
    REQUIRE(date::getDaysInMonth(2, 2022) == 28);
    REQUIRE(date::getDaysInMonth(2, 2020) == 29);
    REQUIRE(date::getDaysInMonth(3, 2022) == 31);
    REQUIRE(date::getDaysInMonth(4, 2022) == 30);
    REQUIRE(date::getDaysInMonth(5, 2022) == 31);
    REQUIRE(date::getDaysInMonth(6, 2022) == 30);
    REQUIRE(date::getDaysInMonth(7, 2022) == 31);
    REQUIRE(date::getDaysInMonth(8, 2022) == 31);
    REQUIRE(date::getDaysInMonth(9, 2022) == 30);
    REQUIRE(date::getDaysInMonth(10, 2022) == 31);
    REQUIRE(date::getDaysInMonth(11, 2022) == 30);
    REQUIRE(date::getDaysInMonth(12, 2022) == 31);
}

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

TEST_CASE("Parse number with 4 digits", "[parsing]")
{
    const std::string s = "2023";
    const auto [i, result] = date::parseNumber<std::uint32_t>(s.begin(), s.end(), 4);
    REQUIRE(i == s.end());
    REQUIRE(result == 2023);
}

TEST_CASE("Parse number with unknown digits", "[parsing]")
{
    const std::string s = "123";
    const auto [i, result] = date::parseNumber<std::uint32_t>(s.begin(), s.end());
    REQUIRE(i == s.end());
    REQUIRE(result == 123);
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

TEST_CASE("UTC time zone", "[parsing]")
{
    const std::string s = "20230202T101112Z";
    const auto result = date::parse(s);
    REQUIRE(result.year == 2023);
    REQUIRE(result.month == 2);
    REQUIRE(result.day == 2);
    REQUIRE(result.hour == 10);
    REQUIRE(result.minute == 11);
    REQUIRE(result.second == 12);
    REQUIRE(result.timeZone == 0);
}

TEST_CASE("Second fraction", "[parsing]")
{
    const std::string s = "20230202T101112.1234Z";
    const auto result = date::parse(s);
    REQUIRE(result.year == 2023);
    REQUIRE(result.month == 2);
    REQUIRE(result.day == 2);
    REQUIRE(result.hour == 10);
    REQUIRE(result.minute == 11);
    REQUIRE(result.second == 12);
    REQUIRE(result.secondFraction == 1234);
    REQUIRE(result.timeZone == 0);
}
