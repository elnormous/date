#ifndef DATE_HPP
#define DATE_HPP

#include <stdexcept>
#include <string>

namespace date
{
    class ParseError final: public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    struct Date final
    {
        std::size_t year;
        std::size_t month;
        std::size_t day;
    };

    template <class I>
    std::pair<I, std::size_t> parseNumbers(const I begin, const I end, const std::size_t count)
    {
        std::size_t result = 0;
        I iterator = begin;

        for (std::size_t i = 0; i < count; ++i, ++iterator)
            result = result * 10 + ((iterator != end && *iterator >= '0' && *iterator <= '9') ?
                static_cast<std::size_t>((*iterator - '0')) :
                throw ParseError{"Invalid number"});

        return {iterator, result};
    }

    inline Date parse(std::string_view s)
    {
        const auto [yearIterator, year] = parseNumbers(s.begin(), s.end(), 4);
        if (yearIterator == s.end() || *yearIterator != '-')
            throw ParseError{"Invalid date"};

        const auto [monthIterator, month] = parseNumbers(yearIterator + 1, s.end(), 2);
        if (monthIterator == s.end() || *monthIterator != '-')
            throw ParseError{"Invalid date"};

        const auto [dayIterator, day] = parseNumbers(monthIterator + 1, s.end(), 2);

        Date result;
        result.year = year;
        result.month = month;
        result.day = day;

        return result;
    }
}

#endif // DATE_HPP
