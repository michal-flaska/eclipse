#include "eclipse/module.hpp"
#include <Psapi.h>

#pragma comment(lib, "psapi.lib")

namespace eclipse {

	std::optional<module_info> module::get(const std::string& name) {
		HMODULE handle = GetModuleHandleA(name.c_str());
		if (!handle)
			return std::nullopt;

		MODULEINFO mod_info;
		if (!GetModuleInformation(GetCurrentProcess(), handle, &mod_info, sizeof(MODULEINFO)))
			return std::nullopt;

		module_info info;
		info.name = name;
		info.base = reinterpret_cast<address_t>(mod_info.lpBaseOfDll);
		info.size = mod_info.SizeOfImage;

		return info;
	}

	std::optional<module_info> module::get_main() {
		HMODULE handle = GetModuleHandleA(nullptr);
		if (!handle)
			return std::nullopt;

		MODULEINFO mod_info;
		if (!GetModuleInformation(GetCurrentProcess(), handle, &mod_info, sizeof(MODULEINFO)))
			return std::nullopt;

		char name[MAX_PATH];
		if (!GetModuleBaseNameA(GetCurrentProcess(), handle, name, MAX_PATH))
			return std::nullopt;

		module_info info;
		info.name = name;
		info.base = reinterpret_cast<address_t>(mod_info.lpBaseOfDll);
		info.size = mod_info.SizeOfImage;

		return info;
	}

	address_t module::get_base(const std::string& name) {
		auto mod = get(name);
		return mod ? mod->base : 0;
	}

} // namespace eclipse
