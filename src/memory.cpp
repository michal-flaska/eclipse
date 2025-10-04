#include "eclipse/memory.hpp"

namespace eclipse {

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

        bool success = false;
        __try {
            memcpy(reinterpret_cast<void*>(address), buffer, size);
            success = true;
        }
        __finally {
            protect(address, size, old_protect);
        }
        return success;
    }

    bool memory::protect(address_t address, size_t size, DWORD protection, DWORD* old_protection) {
        DWORD temp;
        return VirtualProtect(reinterpret_cast<void*>(address), size, protection,
                            old_protection ? old_protection : &temp) != 0;
    }

    address_t memory::follow_pointer(address_t base, const std::vector<uintptr_t>& offsets) {
        if (!base || offsets.empty())
            return 0;

        address_t addr = base;
        for (size_t i = 0; i < offsets.size(); ++i) {
            addr += offsets[i];

            if (i < offsets.size() - 1) {
                __try {
                    addr = read<address_t>(addr);
                    if (!addr)
                        return 0;
                }
                __except (EXCEPTION_EXECUTE_HANDLER) {
                    return 0;
                }
            }
        }

        return addr;
    }

} // namespace eclipse
