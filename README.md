![eclipse-banner](./images/eclipse-banner.png)
Eclipse is a lightweight header-based C++ internal memory manipulation library primarily created for internal game cheats and penetration testing purposes. It's designed to be a simple starting point for your next educational internal cheat project. You can include it in your repository as a Git submodule.

> [!IMPORTANT]
> This library is intended for educational and research purposes only. Use responsibly and only on software you own or have permission to modify.

## Features

- **Memory Operations**: Read/write primitives with automatic type handling
- **Pattern Scanning**: IDA-style signature scanning with wildcard support
- **Module Management**: Enumerate and query loaded modules
- **Pointer Chains**: Follow multi-level pointer paths
- **Protection Handling**: Automatic memory protection management

## Requirements

- C++17 or higher
- Windows (uses WinAPI)
- CMake 3.15+

## Installation

### As a Git Submodule

```bash
git submodule add https://github.com/michal-flaska/eclipse.git external/eclipse
git submodule update --init --recursive
```

### CMakeLists.txt Integration

```cmake
add_subdirectory(external/eclipse)
target_link_libraries(your_project PRIVATE eclipse)
```

## Usage

```cpp
#include <eclipse/eclipse.hpp>

using namespace eclipse;

// Read memory
int health = memory::read<int>(0x12345678);

// Write memory
memory::write<float>(0x12345678, 100.0f);

// Pattern scanning
auto result = pattern::scan("client.dll", "48 8B 05 ?? ?? ?? ?? 48 8B 88");
if (result) {
    // Pattern found at result.address
}

// Follow pointer chain
std::vector<uintptr_t> offsets = { 0x10, 0x20, 0x30 };
address_t addr = memory::follow_pointer(base, offsets);

// Get module info
auto mod = module::get("client.dll");
if (mod) {
    // Use mod->base, mod->size, etc.
}
```

## Building Examples

```bash
mkdir build && cd build
cmake ..
cmake --build .
```
