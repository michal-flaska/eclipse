#include "eclipse/pattern.hpp"
#include "eclipse/module.hpp"
#include <sstream>

namespace eclipse {

    pattern_result pattern::scan(const std::string& module_name, const std::string& signature) {
        auto mod = module::get(module_name);
        if (!mod)
            return { 0, false };

        return scan(mod->base, mod->size, signature);
    }

    pattern_result pattern::scan(address_t start, size_t size, const std::string& signature) {
        std::vector<byte> pattern_bytes;
        std::vector<bool> pattern_mask;

        if (!parse_signature(signature, pattern_bytes, pattern_mask))
            return { 0, false };

        address_t result = scan_internal(start, size, pattern_bytes.data(),
                                        pattern_mask.data(), pattern_bytes.size());
        return { result, result != 0 };
    }

    pattern_result pattern::scan_masked(const std::string& module_name, const byte* pattern, const char* mask) {
        auto mod = module::get(module_name);
        if (!mod)
            return { 0, false };

        return scan_masked(mod->base, mod->size, pattern, mask);
    }

    pattern_result pattern::scan_masked(address_t start, size_t size, const byte* pattern, const char* mask) {
        if (!pattern || !mask)
            return { 0, false };

        size_t pattern_size = strlen(mask);
        if (pattern_size == 0 || pattern_size > size)
            return { 0, false };

        std::vector<bool> bool_mask(pattern_size);
        for (size_t i = 0; i < pattern_size; ++i)
            bool_mask[i] = (mask[i] == 'x');

        address_t result = scan_internal(start, size, pattern, bool_mask.data(), pattern_size);
        return { result, result != 0 };
    }

    bool pattern::parse_signature(const std::string& signature, std::vector<byte>& bytes, std::vector<bool>& mask) {
        std::istringstream iss(signature);
        std::string token;

        while (iss >> token) {
            if (token == "?" || token == "??") {
                bytes.push_back(0);
                mask.push_back(false);
            }
            else {
                try {
                    unsigned long value = std::stoul(token, nullptr, 16);
                    if (value > 0xFF)
                        return false;
                    bytes.push_back(static_cast<byte>(value));
                    mask.push_back(true);
                }
                catch (...) {
                    return false;
                }
            }
        }

        return !bytes.empty();
    }

    address_t pattern::scan_internal(address_t start, size_t size, const byte* pattern,
                                     const bool* mask, size_t pattern_size) {
        if (!start || !size || !pattern || !mask || !pattern_size || pattern_size > size)
            return 0;

        byte* start_ptr = reinterpret_cast<byte*>(start);
        byte* end_ptr = start_ptr + size - pattern_size;

        __try {
            for (byte* current = start_ptr; current <= end_ptr; ++current) {
                bool found = true;

                for (size_t i = 0; i < pattern_size; ++i) {
                    if (mask[i] && current[i] != pattern[i]) {
                        found = false;
                        break;
                    }
                }

                if (found)
                    return reinterpret_cast<address_t>(current);
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            return 0;
        }

        return 0;
    }

} // namespace eclipse
