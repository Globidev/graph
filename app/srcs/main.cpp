#include "application.hpp"

#include "tools/locale.hpp"

int main(int argc, char **argv) {
    // Makes number more readable on stdout
    std::cout.imbue(locale::group_numbers_locale);

    Application app { argc, argv };
    app.run();

    return 0;
}
