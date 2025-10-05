// this is just a basic usage example

#include <eclipse/eclipse.hpp>
#include <iostream>

int main() {
	using namespace eclipse;

	auto client = module::get("client.dll");
	if (client) {
		std::cout << "client.dll - 0x" << std::hex << client->base << "\n";
		std::cout << "Size: 0x" << client->size << "\n\n";
	}

	auto result = pattern::scan("client.dll", "48 8B 05 ?? ?? ?? ?? 48 8B 88");
	if (result) {
		std::cout << "Pattern found at: 0x" << std::hex << result.address << "\n";
	}

	address_t some_address = 0x12345678;
	int value = memory::read<int>(some_address);
	memory::write<float>(some_address, 100.0f);

	return 0;
}
