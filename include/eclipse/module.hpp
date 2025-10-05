#pragma once
#include "types.hpp"
#include <optional>

namespace eclipse {

	class module {
	public:
		static std::optional<module_info> get(const std::string& name);
		static std::optional<module_info> get_main();
		static address_t get_base(const std::string& name);
	};

} // namespace eclipse
