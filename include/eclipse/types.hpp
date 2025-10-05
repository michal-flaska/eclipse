#pragma once
#include <cstdint>
#include <string>
#include <Windows.h>

namespace eclipse {

	using byte = uint8_t;
	using address_t = uintptr_t;

	struct module_info {
		std::string name;
		address_t base;
		size_t size;
	};

	struct pattern_result {
		address_t address;
		bool found;

		operator bool() const { return found; }
	};

} // namespace eclipse
