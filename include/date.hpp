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
        std::size_t hour = 0;
        std::size_t minute = 0;
        std::size_t second = 0;
        std::size_t timeZone = 0;
    };

    inline bool isLeapYear(std::size_t year) noexcept
    {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }

    inline bool is30DayMonth(std::size_t month) noexcept
    {
        return month == 4 || month == 6 || month == 9 || month == 11;
    }

    inline std::size_t getDaysInMonth(std::size_t month, std::size_t year) noexcept
    {
        return month == 2 ? (isLeapYear(year) ? 29 : 28) :
            is30DayMonth(month) ? 30 : 31;
    }

    inline bool isNumber(const char c) noexcept
    {
        return c >= '0' && c <= '9';
    }

    inline std::size_t charToNumber(const char c)
    {
        return isNumber(c) ?
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
        if (yearIterator == end)
            throw ParseError{"Unexpected end"};

        const bool separators = *yearIterator == '-';

        const auto [monthIterator, month] = parseNumbers(yearIterator + (separators ? 1 : 0), end, 2);
        
        if (month < 1 || month > 12)
            throw ParseError{"Invalid month"};

        if (monthIterator == end)
            throw ParseError{"Unexpected end"};

        if (separators && *monthIterator != '-')
            throw ParseError{"Expected a dash"};

        const auto [dayIterator, day] = parseNumbers(monthIterator + (separators ? 1 : 0), end, 2);

        if (day < 1 || day > getDaysInMonth(month, year))
            throw ParseError{"Invalid day"};

        Date result;
        result.year = year;
        result.month = month;
        result.day = day;

        if (dayIterator != end)
        {
            if (*dayIterator != 'T')
                throw ParseError{"Expected a space"};

            const auto [hourIterator, hour] = parseNumbers(dayIterator + 1, end, 2);
            if (hourIterator == end)
                throw ParseError{"Unexpected end"};

            if (separators && *hourIterator != ':')
                throw ParseError{"Expected a colon"};

            const auto [minuteIterator, minute] = parseNumbers(hourIterator + (separators ? 1 : 0), end, 2);
            if (minuteIterator == end)
                throw ParseError{"Unexpected end"};

            if (separators && *minuteIterator != ':')
                throw ParseError{"Expected a colon"};

            const auto [secondIterator, second] = parseNumbers(minuteIterator + (separators ? 1 : 0), end, 2);

            result.hour = hour;
            result.minute = minute;
            result.second = second;
            
            if (secondIterator != end)
            {
                if (*secondIterator == 'Z')
                    result.timeZone = 0;
                else
                    throw ParseError{"Invalid timezone"};
            }
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
