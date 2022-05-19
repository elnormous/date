#include <cstddef>
#include "catch2/catch.hpp"
#include "date.hpp"

TEST_CASE("Test", "[parsing]")
{
    const auto result = date::parse("2022-01-01");

}
