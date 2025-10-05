#pragma once
#include "types.hpp"
#include <string>
#include <vector>

namespace eclipse {

	class pattern {
	public:
		static pattern_result scan(const std::string& module_name, const std::string& signature);
		static pattern_result scan(address_t start, size_t size, const std::string& signature);

	private:
		static bool parse_signature(const std::string& signature, std::vector<byte>& bytes, std::vector<bool>& mask);
		static address_t scan_internal(address_t start, size_t size, const byte* pattern, const bool* mask, size_t pattern_size);
	};

} // namespace eclipse
