#ifndef DATE_HPP
#define DATE_HPP

#include <cstring>
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

    inline std::size_t charToNumber(const char c)
    {
        return (c >= '0' && c <= '9') ?
            static_cast<std::size_t>(c - '0') :
            throw ParseError{"Invalid number"};
    }

    template <class I>
    std::pair<I, std::size_t> parseNumbers(const I begin, const I end, const std::size_t count)
    {
        std::size_t result = 0;
        I iterator = begin;

        for (std::size_t i = 0; i < count; ++i, ++iterator)
            result = result * 10 + (iterator != end ?
                                    charToNumber(static_cast<char>(*iterator)) :
                                    throw ParseError{"Invalid number"});

        return {iterator, result};
    }

    template <class I>
    inline Date parse(const I begin, const I end)
    {
        const auto [yearIterator, year] = parseNumbers(begin, end, 4);
        if (yearIterator == end || *yearIterator != '-')
            throw ParseError{"Expected a dash"};

        const auto [monthIterator, month] = parseNumbers(yearIterator + 1, end, 2);
        if (monthIterator == end || *monthIterator != '-')
            throw ParseError{"Expected a dash"};

        const auto [dayIterator, day] = parseNumbers(monthIterator + 1, end, 2);

        Date result;
        result.year = year;
        result.month = month;
        result.day = day;

        if (dayIterator != end)
        {
            if (*dayIterator != ' ')
                throw ParseError{"Expected a space"};

            const auto [hourIterator, hour] = parseNumbers(dayIterator + 1, end, 2);
            if (hourIterator == end || *hourIterator != ':')
                throw ParseError{"Expected a dash"};

            const auto [minuteIterator, minute] = parseNumbers(hourIterator + 1, end, 2);
            if (minuteIterator == end || *minuteIterator != ':')
                throw ParseError{"Expected a dash"};

            const auto [secondIterator, second] = parseNumbers(minuteIterator + 1, end, 2);

            if (secondIterator != end)
                throw ParseError{"Invalid date"};

            result.hour = hour;
            result.minute = minute;
            result.second = second;
        }

        return result;
    }

    template <class T>
    inline Date parse(const T& s)
    {
        using std::begin, std::end;
        return parse(begin(s), end(s));
    }

    inline Date parse(const char* s)
    {
        return parse(s, s + std::strlen(s));
    }
}

#endif // DATE_HPP
