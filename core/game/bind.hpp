#pragma once
#include <cstdint>
#include <cstddef>
#include <type_traits>

inline auto operator_new = (void* (__cdecl*)(std::size_t))(0x0063eb87);
inline auto operator_delete = (void(__cdecl*)(void*))(0x0063f514);
inline auto _realloc = (void* (__cdecl*)(void*, std::size_t))(0x0063f73a);

// A macro that automatically replaces the new and delete operators for the containing type with Mater-National's.
#define REPLACE_OPERATOR_NEW_DELETE __forceinline void* operator new(std::size_t sz) { return operator_new(sz); } __forceinline void operator delete(void* ptr) { return operator_delete(ptr); } \

#define UNIMPLEMENTED(ptr) constexpr static std::uint32_t addr = ##ptr; __asm jmp addr