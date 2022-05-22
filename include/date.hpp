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
        std::size_t hour;
        std::size_t minute;
        std::size_t second;
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
            throw ParseError{"Expected a dash"};

        const auto [monthIterator, month] = parseNumbers(yearIterator + 1, s.end(), 2);
        if (monthIterator == s.end() || *monthIterator != '-')
            throw ParseError{"Expected a dash"};

        const auto [dayIterator, day] = parseNumbers(monthIterator + 1, s.end(), 2);

        Date result;
        result.year = year;
        result.month = month;
        result.day = day;

        if (dayIterator != s.end())
        {
            if (*dayIterator != ' ')
                throw ParseError{"Expected a space"};

            const auto [hourIterator, hour] = parseNumbers(dayIterator + 1, s.end(), 2);
            if (hourIterator == s.end() || *hourIterator != ':')
                throw ParseError{"Expected a dash"};

            const auto [minuteIterator, minute] = parseNumbers(hourIterator + 1, s.end(), 2);
            if (minuteIterator == s.end() || *minuteIterator != ':')
                throw ParseError{"Expected a dash"};

            const auto [secondIterator, second] = parseNumbers(minuteIterator + 1, s.end(), 2);

            if (secondIterator != s.end())
                throw ParseError{"Invalid date"};

            result.hour = hour;
            result.minute = minute;
            result.second = second;
        }

        return result;
    }
}

#endif // DATE_HPP
