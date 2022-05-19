#ifndef DATE_HPP
#define DATE_HPP

#include <chrono>
#include <string>

namespace date
{
    std::chrono::system_clock parse(std::string_view s)
    {
        return std::chrono::system_clock{};
    }
}

#endif // DATE_HPP
