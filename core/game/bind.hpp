#pragma once
#include <cstdint>
#include <cstddef>
#include <type_traits>

inline auto operator_new = (void* (__cdecl*)(std::size_t))(0x0063eb87);
inline auto operator_delete = (void(__cdecl*)(void*))(0x0063f514);

/*
A base class to replace the new and delete operators with Mater-National's. Mater-National classes should extend this.
*/
class MaterManagedObject {
public:
	__forceinline void* operator new(std::size_t sz) {
		return operator_new(sz);
	}
	__forceinline void operator delete(void* ptr) {
		return operator_delete(ptr);
	}
};

#define UNIMPLEMENTED(ptr) constexpr std::uint32_t addr = ##ptr; __asm jmp addr