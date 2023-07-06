#include <cstddef>

namespace iocod::Memory {

inline constexpr std::size_t kDefaultAlignment = 16;
[[nodiscard]] void* Alloc(std::size_t size, std::size_t alignment = kDefaultAlignment);

}

void* __cdecl operator new[](const size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	return iocod::Memory::Alloc(size);
}
