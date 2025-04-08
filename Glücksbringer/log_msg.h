#pragma once

#include <stacktrace>
#include <source_location>
#include <string_view>
#include <chrono>
#include <iostream>

#include "datetime.h"


class Logerr {

public:
    static void log_msg(const std::string_view message, const std::source_location &location = std::source_location::current(),const std::chrono::system_clock::time_point &tp = std::chrono::system_clock::now());
    void logx() {
        std::cout << std::stacktrace::current() << '\n';
    }
    static void log_rdseed(unsigned __int64 nr, const std::chrono::system_clock::time_point& tp = std::chrono::system_clock::now());
    static void log_euromil(const std::vector<int> &nr, const std::vector<int>& star, const std::chrono::system_clock::time_point& tpt, const std::chrono::system_clock::time_point& tp = std::chrono::system_clock::now()); //-->tpt next play day, gen_version param ?
    static void log_cout(const std::string_view& message, const std::source_location& location = std::source_location::current());
    static void log_web(const std::string_view& message, const std::chrono::year_month_day& ymd, const std::source_location& location = std::source_location::current(), const std::chrono::system_clock::time_point& tp = std::chrono::system_clock::now());
};