#include "eclipse/memory.hpp"

namespace eclipse {

    template<typename T>
    T memory::read_protected(address_t address) {
        DWORD old_protect;
        if (!protect(address, sizeof(T), PAGE_EXECUTE_READWRITE, &old_protect))
            return T{};

        T value = read<T>(address);
        protect(address, sizeof(T), old_protect);
        return value;
    }

    template<typename T>
    void memory::write_protected(address_t address, const T& value) {
        DWORD old_protect;
        if (!protect(address, sizeof(T), PAGE_EXECUTE_READWRITE, &old_protect))
            return;

        write<T>(address, value);
        protect(address, sizeof(T), old_protect);
    }

    bool memory::read_bytes(address_t address, void* buffer, size_t size) {
        if (!address || !buffer || !size)
            return false;

        __try {
            memcpy(buffer, reinterpret_cast<void*>(address), size);
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
    }

    bool memory::write_bytes(address_t address, const void* buffer, size_t size) {
        if (!address || !buffer || !size)
            return false;

        DWORD old_protect;
        if (!protect(address, size, PAGE_EXECUTE_READWRITE, &old_protect))
            return false;

        memcpy(reinterpret_cast<void*>(address), buffer, size);
        protect(address, size, old_protect);
        return true;
    }

    bool memory::protect(address_t address, size_t size, DWORD protection, DWORD* old_protection) {
        DWORD temp;
        return VirtualProtect(reinterpret_cast<void*>(address), size, protection, old_protection ? old_protection : &temp) != 0;
    }

    address_t memory::follow_pointer(address_t base, const std::vector<uintptr_t>& offsets) {
        address_t addr = base;

        for (size_t i = 0; i < offsets.size(); ++i) {
            if (!addr)
                return 0;

            addr = read<address_t>(addr);
            if (i < offsets.size() - 1 || offsets[i] != 0)
                addr += offsets[i];
        }

        return addr;
    }

    // Explicit template instantiations for common types
    template int memory::read_protected<int>(address_t);
    template float memory::read_protected<float>(address_t);
    template double memory::read_protected<double>(address_t);
    template address_t memory::read_protected<address_t>(address_t);

    template void memory::write_protected<int>(address_t, const int&);
    template void memory::write_protected<float>(address_t, const float&);
    template void memory::write_protected<double>(address_t, const double&);
    template void memory::write_protected<address_t>(address_t, const address_t&);

} // namespace eclipse
