#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>


#define BOLD "\e[1m"
#define YELLOW "\e[38;2;180;180;20m"
#define RED "\e[38;2;220;50;50m"
#define CLEAR "\e[0m"


#define LOG_WARNING(warning) std::cerr << YELLOW << BOLD << "[WARNING] " << CLEAR << YELLOW << warning << CLEAR << std::endl
#define LOG_ERROR(error) std::cerr << RED << BOLD << "[ERROR] " << CLEAR << RED << error << CLEAR << std::endl


#define ASSERT_FATAL_ERROR(predicate, error_str)                        \
if (!(predicate)) {                                                     \
    std::stringstream error;                                            \
    error << "Assertion `" << #predicate << "` failed: " << error_str;  \
    LOG_ERROR(error.str());                                             \
    throw std::runtime_error(error.str());                              \
}
