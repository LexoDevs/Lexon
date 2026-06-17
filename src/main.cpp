#include "Engine.h"


int main() {

    try {
       Engine engine;
       engine.runEngine();
    }
    catch (const std::exception& error) {

        std::cerr << error.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

}

