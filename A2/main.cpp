#include <windows.h>
#include "game.hpp"
int main() {
    // SetDllDirectory(TEXT("bin"));
    // SetDllDirectoryA("bin");  // Use ANSI version
    LoadLibraryA("bin/libgcc_s_dw2-1.dll");
    LoadLibraryA("bin/libstdc++-6.dll");
    // SetDllDirectoryW(L"bin");  // Use Unicode version
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