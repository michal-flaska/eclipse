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
        info.handle = handle;

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
        GetModuleBaseNameA(GetCurrentProcess(), handle, name, MAX_PATH);

        module_info info;
        info.name = name;
        info.base = reinterpret_cast<address_t>(mod_info.lpBaseOfDll);
        info.size = mod_info.SizeOfImage;
        info.handle = handle;

        return info;
    }

    std::vector<module_info> module::get_all() {
        std::vector<module_info> modules;
        HMODULE handles[1024];
        DWORD needed;

        if (!EnumProcessModules(GetCurrentProcess(), handles, sizeof(handles), &needed))
            return modules;

        int count = needed / sizeof(HMODULE);
        modules.reserve(count);

        for (int i = 0; i < count; ++i) {
            MODULEINFO mod_info;
            char name[MAX_PATH];

            if (GetModuleInformation(GetCurrentProcess(), handles[i], &mod_info, sizeof(MODULEINFO)) &&
                GetModuleBaseNameA(GetCurrentProcess(), handles[i], name, MAX_PATH)) {

                module_info info;
                info.name = name;
                info.base = reinterpret_cast<address_t>(mod_info.lpBaseOfDll);
                info.size = mod_info.SizeOfImage;
                info.handle = handles[i];

                modules.push_back(info);
            }
        }

        return modules;
    }

    address_t module::get_base(const std::string& name) {
        auto mod = get(name);
        return mod ? mod->base : 0;
    }

    bool module::is_loaded(const std::string& name) {
        return GetModuleHandleA(name.c_str()) != nullptr;
    }

} // namespace eclipse
