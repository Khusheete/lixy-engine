#include "application.hpp"

#include <exception>
#include <iostream>


int main() {
    DemoApplication app{};

    try {
        app.main_loop();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}