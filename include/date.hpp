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
        std::uint32_t year;
        std::uint32_t month;
        std::uint32_t day;
        std::uint32_t hour = 0;
        std::uint32_t minute = 0;
        std::uint32_t second = 0;
        std::uint32_t secondFraction = 0;
        std::uint32_t timeZone = 0;
    };

    inline bool isLeapYear(std::uint32_t year) noexcept
    {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }

    inline bool is30DayMonth(std::uint32_t month) noexcept
    {
        return month == 4 || month == 6 || month == 9 || month == 11;
    }

    inline std::uint32_t getDaysInMonth(std::uint32_t month, std::uint32_t year) noexcept
    {
        return month == 2 ? (isLeapYear(year) ? 29 : 28) :
            is30DayMonth(month) ? 30 : 31;
    }

    inline bool isNumber(const char c) noexcept
    {
        return c >= '0' && c <= '9';
    }

    inline std::uint32_t charToNumber(const char c)
    {
        return isNumber(c) ?
            static_cast<std::uint32_t>(c - '0') :
            throw ParseError{"Invalid number"};
    }

    template <class I>
    std::pair<I, std::uint32_t> parseNumber(const I begin, const I end, const std::size_t digits = 0)
    {
        std::uint32_t result = 0;
        I iterator = begin;

        for (std::size_t i = 0; digits == 0 || i < digits; ++i, ++iterator)
        {
            if (digits == 0 && (iterator == end || !isNumber(*iterator)))
                break;

            result = result * 10U + (iterator != end ?
                                     charToNumber(static_cast<char>(*iterator)) :
                                     throw ParseError{"Invalid number"});
        }

        return {iterator, result};
    }

    template <class I>
    inline Date parse(const I begin, const I end)
    {
        const auto [yearIterator, year] = parseNumber(begin, end, 4);
        if (yearIterator == end)
            throw ParseError{"Unexpected end"};

        const bool separators = *yearIterator == '-';

        const auto [monthIterator, month] = parseNumber(yearIterator + (separators ? 1 : 0), end, 2);
        
        if (month < 1 || month > 12)
            throw ParseError{"Invalid month"};

        if (monthIterator == end)
            throw ParseError{"Unexpected end"};

        if (separators && *monthIterator != '-')
            throw ParseError{"Expected a dash"};

        const auto [dayIterator, day] = parseNumber(monthIterator + (separators ? 1 : 0), end, 2);

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

            const auto [hourIterator, hour] = parseNumber(dayIterator + 1, end, 2);
            if (hourIterator == end)
                throw ParseError{"Unexpected end"};

            if (separators && *hourIterator != ':')
                throw ParseError{"Expected a colon"};

            const auto [minuteIterator, minute] = parseNumber(hourIterator + (separators ? 1 : 0), end, 2);
            if (minuteIterator == end)
                throw ParseError{"Unexpected end"};

            if (separators && *minuteIterator != ':')
                throw ParseError{"Expected a colon"};

            const auto [secondIterator, second] = parseNumber(minuteIterator + (separators ? 1 : 0), end, 2);

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
