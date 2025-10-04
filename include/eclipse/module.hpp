#pragma once
#include "types.hpp"
#include <optional>

namespace eclipse {

    class module {
    public:
        // Get module by name
        static std::optional<module_info> get(const std::string& name);

        // Get main module (exe)
        static std::optional<module_info> get_main();

        // Get all loaded modules
        static std::vector<module_info> get_all();

        // Get module base address
        static address_t get_base(const std::string& name);

        // Check if module is loaded
        static bool is_loaded(const std::string& name);
    };

} // namespace eclipse
