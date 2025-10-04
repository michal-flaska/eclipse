#pragma once
#include "types.hpp"
#include <string>
#include <optional>

namespace eclipse {

    class pattern {
    public:
        // Scan for pattern in module
        static pattern_result scan(const std::string& module_name, const std::string& signature);

        // Scan for pattern in address range
        static pattern_result scan(address_t start, size_t size, const std::string& signature);

        // Scan for pattern in module with mask
        static pattern_result scan_masked(const std::string& module_name, const byte* pattern, const char* mask);

        // Scan for pattern in address range with mask
        static pattern_result scan_masked(address_t start, size_t size, const byte* pattern, const char* mask);

    private:
        // Parse IDA-style signature (e.g., "48 8B 05 ?? ?? ?? ??")
        static bool parse_signature(const std::string& signature, std::vector<byte>& bytes, std::vector<bool>& mask);

        // Internal scan implementation
        static address_t scan_internal(address_t start, size_t size, const byte* pattern, const bool* mask, size_t pattern_size);
    };

} // namespace eclipse
