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
    std::pair<I, std::size_t> parseYear(const I begin, const I end)
    {
        std::size_t result = 0;
        I iterator = begin;
        result += (iterator != end && *iterator >= '0' && *iterator <= '9') ?
            static_cast<std::size_t>((*iterator - '0') * 1000) :
            throw ParseError{"Invalid year"};

        result += (++iterator != end && *iterator >= '0' && *iterator <= '9') ?
            static_cast<std::size_t>((*iterator - '0') * 100) :
            throw ParseError{"Invalid year"};

        result += (++iterator != end && *iterator >= '0' && *iterator <= '9') ?
            static_cast<std::size_t>((*iterator - '0') * 10) :
            throw ParseError{"Invalid year"};

        result += (++iterator != end && *iterator >= '0' && *iterator <= '9') ?
            static_cast<std::size_t>(*iterator - '0') :
            throw ParseError{"Invalid year"};

        return {++iterator, result};
    }

    Date parse(std::string_view s)
    {
        Date result;
        result.year = 2022;
        result.month = 01;
        result.day = 01;

        return result;
    }
}

#endif // DATE_HPP
