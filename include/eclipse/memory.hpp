#pragma once
#include "types.hpp"
#include <type_traits>

namespace eclipse {

    class memory {
    public:
        // Read memory - inline for any type
        template<typename T>
        static T read(address_t address) {
            static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
            return *reinterpret_cast<T*>(address);
        }

        // Write memory - inline for any type
        template<typename T>
        static void write(address_t address, const T& value) {
            static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
            *reinterpret_cast<T*>(address) = value;
        }

        // Read with protection change
        template<typename T>
        static T read_protected(address_t address) {
            static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
            DWORD old_protect;
            if (!protect(address, sizeof(T), PAGE_EXECUTE_READWRITE, &old_protect))
                return T{};

            T value = T{};
            __try {
                value = read<T>(address);
            }
            __finally {
                protect(address, sizeof(T), old_protect);
            }
            return value;
        }

        // Write with protection change
        template<typename T>
        static void write_protected(address_t address, const T& value) {
            static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
            DWORD old_protect;
            if (!protect(address, sizeof(T), PAGE_EXECUTE_READWRITE, &old_protect))
                return;

            __try {
                write<T>(address, value);
            }
            __finally {
                protect(address, sizeof(T), old_protect);
            }
        }

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
