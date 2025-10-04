#include <eclipse/eclipse.hpp>
#include <iostream>

int main() {
    using namespace eclipse;

    // Get main module info
    auto main_module = module::get_main();
    if (main_module) {
        std::cout << "Main module: " << main_module->name << "\n";
        std::cout << "Base address: 0x" << std::hex << main_module->base << "\n";
        std::cout << "Size: 0x" << main_module->size << "\n\n";
    }

    // List all loaded modules
    std::cout << "Loaded modules:\n";
    auto modules = module::get_all();
    for (const auto& mod : modules) {
        std::cout << "  " << mod.name << " - 0x" << std::hex << mod.base << "\n";
    }
    std::cout << "\n";

    // Pattern scanning example
    auto result = pattern::scan("client.dll", "48 8B 05 ?? ?? ?? ?? 48 8B 88");
    if (result) {
        std::cout << "Pattern found at: 0x" << std::hex << result.address << "\n";
    }

    // Memory read/write example
    address_t some_address = 0x12345678; // Replace with actual address
    int value = memory::read<int>(some_address);
    std::cout << "Value at address: " << std::dec << value << "\n";

    // Pointer chain example
    std::vector<uintptr_t> offsets = { 0x10, 0x20, 0x30 };
    address_t final_address = memory::follow_pointer(some_address, offsets);
    std::cout << "Final address: 0x" << std::hex << final_address << "\n";

    return 0;
}
