#pragma once

class c_call
{
	uintptr_t hook_address, original_address;
public:
	void install(uintptr_t address, void *function)
	{
		this->hook_address = address;

		unsigned long old_protection;
		VirtualProtect(reinterpret_cast<void *>(address), 5, PAGE_EXECUTE_READWRITE, &old_protection);

		if (*reinterpret_cast<uint8_t *>(address) == 0xE8)
		{
			original_address = *reinterpret_cast<uintptr_t *>(address + 1) + address + 5;
			*reinterpret_cast<uintptr_t *>(address + 1) =
				reinterpret_cast<uintptr_t>(function) - address - 5;
		}

		VirtualProtect(reinterpret_cast<void *>(address), 5, old_protection, &old_protection);
	}

	uintptr_t original()
	{
		return original_address;
	}

	void restore()
	{
		unsigned long old_protection;
		VirtualProtect(reinterpret_cast<void *>(hook_address), 5, PAGE_EXECUTE_READWRITE, &old_protection);

		*reinterpret_cast<uintptr_t *>(hook_address + 1) = original_address - hook_address - 5;

		VirtualProtect(reinterpret_cast<void *>(hook_address), 5, old_protection, &old_protection);
	}
};
