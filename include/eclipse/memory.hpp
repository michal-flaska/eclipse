#pragma once
#include "types.hpp"
#include <type_traits>

namespace eclipse {

	class memory {
	public:
		template<typename T>
		static T read(address_t address) {
			static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
			return *reinterpret_cast<T*>(address);
		}

		template<typename T>
		static void write(address_t address, const T& value) {
			static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
			*reinterpret_cast<T*>(address) = value;
		}
	};

} // namespace eclipse
