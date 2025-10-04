#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <Windows.h>

namespace eclipse {

    // Common types
    using byte = uint8_t;
    using address_t = uintptr_t;

    // Module information
    struct module_info {
        std::string name;
        address_t base;
        size_t size;
        HMODULE handle;
    };

    // Pattern scan result
    struct pattern_result {
        address_t address;
        bool found;

        operator bool() const { return found; }
    };

} // namespace eclipse
