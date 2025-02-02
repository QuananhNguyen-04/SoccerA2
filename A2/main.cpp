#include <windows.h>
#include "game.hpp"
int main() {
    try {
        Game g = Game();
        g.run();
    }
    catch (const std::exception& e){
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;  // Exit with failure code
    }
    return EXIT_SUCCESS;
}