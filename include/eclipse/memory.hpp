#pragma once
#include "types.hpp"
#include <type_traits>

namespace eclipse {

    class memory {
    public:
        // Read memory
        template<typename T>
        static T read(address_t address) {
            return *reinterpret_cast<T*>(address);
        }

        // Write memory
        template<typename T>
        static void write(address_t address, const T& value) {
            *reinterpret_cast<T*>(address) = value;
        }

        // Read with protection change
        template<typename T>
        static T read_protected(address_t address);

        // Write with protection change
        template<typename T>
        static void write_protected(address_t address, const T& value);

        // Read array/buffer
        static bool read_bytes(address_t address, void* buffer, size_t size);

        // Write array/buffer
        static bool write_bytes(address_t address, const void* buffer, size_t size);

        // Change memory protection
        static bool protect(address_t address, size_t size, DWORD protection, DWORD* old_protection = nullptr);

        // Follow pointer chain
        static address_t follow_pointer(address_t base, const std::vector<uintptr_t>& offsets);
    };

} // namespace eclipse
