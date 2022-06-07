#ifndef DATE_HPP
#define DATE_HPP

#include <cstring>
#include <limits>
#include <stdexcept>
#include <string>

namespace date
{
    class ParseError final: public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    using Year = std::uint16_t;
    using Month = std::uint8_t;
    using Day = std::uint8_t;
    using Hour = std::uint8_t;
    using Minute = std::uint8_t;
    using Second = std::uint8_t;

    struct DateTime final
    {
        Year year;
        Month month;
        Day day;
        Hour hour = 0;
        Minute minute = 0;
        Second second = 0;
        std::uint32_t secondFraction = 0;
        std::uint32_t timeZone = 0;
    };

    inline bool isLeapYear(const Year year) noexcept
    {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }

    inline bool is30DayMonth(const Month month) noexcept
    {
        return month == 4 || month == 6 || month == 9 || month == 11;
    }

    inline std::uint32_t getDaysInMonth(const Month month, const Year year) noexcept
    {
        return month == 2 ? (isLeapYear(year) ? 29 : 28) :
            is30DayMonth(month) ? 30 : 31;
    }

    inline bool isDigit(const char c) noexcept
    {
        return c >= '0' && c <= '9';
    }

    template <class R>
    inline R charToNumber(const char c)
    {
        return isDigit(c) ?
            static_cast<R>(c - '0') :
            throw ParseError{"Invalid number"};
    }

    template <class R, class I>
    std::pair<I, R> parseNumber(const I begin, const I end, const std::size_t digits = 0)
    {
        R result = 0;
        I iterator = begin;

        for (std::size_t i = 0; digits == 0 || i < digits; ++i, ++iterator)
        {
            if (digits == 0 && (iterator == end || !isDigit(static_cast<char>(*iterator))))
                break;

            if (iterator == end)
                throw ParseError{"Invalid number"};

            if (result > std::numeric_limits<R>::max() / R(10U))
                throw ParseError{"Number too large"};

            result *= R(10U);

            const auto number = charToNumber<R>(static_cast<char>(*iterator));

            if (~number < result && R(result + number) < number)
                throw ParseError{"Number too large"};

            result += number;
        }

        return {iterator, result};
    }

    template <class I>
    inline DateTime parse(const I begin, const I end)
    {
        const auto [yearIterator, year] = parseNumber<std::uint16_t>(begin, end, 4);
        if (yearIterator == end)
            throw ParseError{"Unexpected end"};

        const bool separators = *yearIterator == '-';

        const auto [monthIterator, month] = parseNumber<std::uint8_t>(yearIterator + (separators ? 1 : 0), end, 2);
        
        if (month < 1 || month > 12)
            throw ParseError{"Invalid month"};

        if (monthIterator == end)
            throw ParseError{"Unexpected end"};

        if (separators && *monthIterator != '-')
            throw ParseError{"Expected a dash"};

        const auto [dayIterator, day] = parseNumber<std::uint8_t>(monthIterator + (separators ? 1 : 0), end, 2);

        if (day < 1 || day > getDaysInMonth(month, year))
            throw ParseError{"Invalid day"};

        DateTime result;
        result.year = year;
        result.month = month;
        result.day = day;

        if (dayIterator != end)
        {
            if (*dayIterator != 'T')
                throw ParseError{"Expected a space"};

            const auto [hourIterator, hour] = parseNumber<std::uint8_t>(dayIterator + 1, end, 2);
            if (hourIterator == end)
                throw ParseError{"Unexpected end"};

            if (separators && *hourIterator != ':')
                throw ParseError{"Expected a colon"};

            const auto [minuteIterator, minute] = parseNumber<std::uint8_t>(hourIterator + (separators ? 1 : 0), end, 2);
            if (minuteIterator == end)
                throw ParseError{"Unexpected end"};

            if (separators && *minuteIterator != ':')
                throw ParseError{"Expected a colon"};

            const auto [secondIterator, second] = parseNumber<std::uint8_t>(minuteIterator + (separators ? 1 : 0), end, 2);

            result.hour = hour;
            result.minute = minute;
            result.second = second;
            
            if (secondIterator != end)
            {
                auto iterator = secondIterator;

                if (*iterator == '.')
                {
                    const auto [secondFractionIterator, secondFraction] = parseNumber<std::uint32_t>(iterator + 1, end);
                    result.secondFraction = secondFraction;
                    iterator = secondFractionIterator;
                }

                if (*iterator == 'Z')
                    result.timeZone = 0;
                else
                    throw ParseError{"Invalid timezone"};
            }
        }

        return result;
    }

    template <class T>
    inline DateTime parse(const T& s)
    {
        using std::begin, std::end;
        return parse(begin(s), end(s));
    }

    inline DateTime parse(const char* s)
    {
        return parse(s, s + std::strlen(s));
    }
}

#endif // DATE_HPP
