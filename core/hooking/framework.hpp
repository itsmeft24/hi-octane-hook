#pragma once

#include <iostream>
#include <format>
#include <cstdint>
#include <Windows.h>
#include <detours/detours.h>

#include "relocate_code.hpp"

#define DefineReplacementHook(name) \
struct name : public hooking::impl::ReplacementHook<name>

#define DefineInlineHook(name) \
struct name : public hooking::impl::InlineHook<name>

template <class T> constexpr bool is_ptr = std::is_integral_v<T> || std::is_pointer_v<T>;

namespace winapi {
    enum class Perm : DWORD {
        None = PAGE_NOACCESS,
        Read = PAGE_READONLY,
        ReadWrite = PAGE_READWRITE,
        WriteCopy = PAGE_WRITECOPY,
        Execute = PAGE_EXECUTE,
        ExecuteRead = PAGE_EXECUTE_READ,
        ExecuteReadWrite = PAGE_EXECUTE_READWRITE,
        ExecuteWriteCopy = PAGE_EXECUTE_WRITECOPY,
        Guard = PAGE_GUARD,
        NoCache = PAGE_NOCACHE,
        WriteCombine = PAGE_WRITECOMBINE,
    };
    // Sets the desired permission on the memory block.
    template <typename T, typename std::enable_if_t<is_ptr<T>>* = nullptr>
    inline std::pair<Perm, bool> set_permission(T ptr, size_t size, Perm perm) {
        Perm old_perm = Perm::None;
        bool success = static_cast<bool>(VirtualProtect(reinterpret_cast<void*>(ptr), size, static_cast<DWORD>(perm), reinterpret_cast<DWORD*>(&old_perm)));
        return std::make_pair(old_perm, success);
    }

    template <typename T> inline T* allocate_code(size_t count) {
        return reinterpret_cast<T*>(VirtualAlloc(NULL, sizeof(T) * count, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
    }
};

namespace hooking {

    template <typename T1, typename std::enable_if_t<is_ptr<T1>>* = nullptr, typename T2, typename std::enable_if_t<is_ptr<T2>>* = nullptr>
    inline void write_jmp(T1 src, T2 dst) {
        uintptr_t relativeAddress = (uintptr_t)((std::uint8_t*)dst - (uintptr_t)src) - 5;

        winapi::set_permission(src, 5, winapi::Perm::ExecuteReadWrite);
        *(std::uint8_t*)src = 0xE9;
        *(uintptr_t*)((uintptr_t)src + 1) = relativeAddress;
    }

    template <typename T1, typename std::enable_if_t<is_ptr<T1>>* = nullptr, typename T2, typename std::enable_if_t<is_ptr<T2>>* = nullptr>
    inline void write_call(T1 src, T2 dst) {
        uintptr_t relativeAddress = (uintptr_t)((std::uint8_t*)dst - (uintptr_t)src) - 5;

        winapi::set_permission(src, 5, winapi::Perm::ExecuteReadWrite);
        *(std::uint8_t*)src = 0xE8;
        *(uintptr_t*)((uintptr_t)src + 1) = relativeAddress;
    }

    template <typename T1, typename std::enable_if_t<is_ptr<T1>>* = nullptr, typename T2, typename std::enable_if_t<is_ptr<T2>>* = nullptr>
    inline void write_push(T1 src, T2 dst) {
        winapi::set_permission(src, 5, winapi::Perm::ExecuteReadWrite);
        *(std::uint8_t*)src = 0x68;
        *(uintptr_t*)((uintptr_t)src + 1) = (uintptr_t)dst;
    }

    template <typename T, typename std::enable_if_t<is_ptr<T>>* = nullptr>
    inline bool write_nop(T addr, std::size_t code_size) {
        const auto& [original_protection, success] = winapi::set_permission(addr, code_size, winapi::Perm::ExecuteReadWrite);
        if (success) {
            std::memset(reinterpret_cast<void*>(addr), 0x90, code_size);
            return true;
        }
        return false;
    }

    namespace legacy {
        template <typename T1, typename std::enable_if_t<is_ptr<T1>>* = nullptr, typename T2, typename std::enable_if_t<is_ptr<T2>>* = nullptr>
        inline void inline_replace(T1 src, T2 dst, std::size_t size) {
            write_nop(src, size);
            write_call(src, dst);
        }
    };

    union Register {
        void* pointer;
        std::uint32_t unsigned_integer;
        std::int32_t signed_integer;
        float floating_point;
    };

    struct InlineCtx {
		Register eflags;
		Register edi;
		Register esi;
		Register ebp;
		Register esp;
		Register ebx;
		Register edx;
		Register ecx;
		Register eax;

        inline std::string to_string() {
            return std::format("eax: {:#X}\necx: {:#X}\nedx: {:#X}\nebx: {:#X}\nesp: {:#X}\nebp: {:#X}\nesi: {:#X}\nedi: {:#X}\neflags: {:#X}\n",
                eax.unsigned_integer,
                ecx.unsigned_integer,
                edx.unsigned_integer,
                ebx.unsigned_integer,
                esp.unsigned_integer,
                ebp.unsigned_integer,
                esi.unsigned_integer,
                edi.unsigned_integer,
                eflags.unsigned_integer
            );
        }
    };

    static_assert(sizeof(InlineCtx) == 36);

    template<typename R, typename... A>
    using GenericFuncPtr = R(*)(A...);

    namespace impl {

        template<typename Derived>
        class ReplacementHook {

            template<typename T = Derived>
            using CallbackFuncPtr = decltype(&T::callback);

            static inline auto& orig_ref() {
                static constinit CallbackFuncPtr<Derived> s_func_ptr = nullptr;

                return s_func_ptr;
            }

        public:
            template<typename... Args>
            static inline decltype(auto) original(Args &&... args) {
                return orig_ref()(std::forward<Args>(args)...);
            }

            template<typename R, typename ...A>
            static inline void install_at_func_ptr(GenericFuncPtr<R, A...> ptr) {
                using ArgFuncPtr = decltype(ptr);

                static_assert(std::is_same_v<ArgFuncPtr, CallbackFuncPtr<>>, "Argument pointer type must match callback type!");

                orig_ref() = ptr;

                DetourTransactionBegin();
                DetourUpdateThread(GetCurrentThread());
                DetourAttach(reinterpret_cast<void**>(&orig_ref()), Derived::callback);
                DetourTransactionCommit();
            }

            static inline void install_at_ptr(uintptr_t ptr) {

                orig_ref() = CallbackFuncPtr<>(ptr);

                DetourTransactionBegin();
                DetourUpdateThread(GetCurrentThread());
                DetourAttach(reinterpret_cast<void**>(&orig_ref()), Derived::callback);
                DetourTransactionCommit();
            }

            static inline void uninstall() {
                DetourTransactionBegin();
                DetourUpdateThread(GetCurrentThread());
                DetourDetach(reinterpret_cast<void**>(&orig_ref()), Derived::callback);
                DetourTransactionCommit();
            }
        };

        template<typename Derived>
        class InlineHook {

            template<typename T = Derived>
            using CallbackFuncPtr = decltype(&T::callback);

        public:

            static inline void install_at_ptr(uintptr_t ptr) {
                static_assert(std::is_same_v<void(__cdecl*)(InlineCtx&), CallbackFuncPtr<>>, "Callback function must be void and take an InlineCtx!");

                // Calculate the minimum bytes needed to be backed up, and an upper-bound limit of how many bytes the relocated code could take. (Used for below allocation)
                auto [original_code_len, padded_code_len] = find_suitable_backup_size(ptr);
                
                if (original_code_len < 5) throw std::exception();
				
                // Allocate code for inline handler.
                std::uint8_t* jit_area = winapi::allocate_code<std::uint8_t>(11 + padded_code_len + 5);
                
                // Build inline handler.
                *jit_area = 0x60;
                jit_area++; // pushad
                *jit_area = 0x9C;
                jit_area++; // pushfd
                *jit_area = 0x54;
                jit_area++; // push esp
                write_call(jit_area, Derived::callback);
                jit_area += 5; // call Derived::callback
                *jit_area = 0x58;
                jit_area++; // pop eax
                *jit_area = 0x9D;
                jit_area++; // popfd
                *jit_area = 0x61;
                jit_area++; // popad
                
                // Attempt to build/relocate the code, and if successful, copy into the trampoline.
                auto relocated = relocate_code(ptr, original_code_len, reinterpret_cast<uintptr_t>(jit_area)).unwrap();
                std::memcpy(jit_area, relocated.data(), relocated.size());
                jit_area += relocated.size();
                write_jmp(jit_area, ptr + 5); jit_area += 5; // jmp ptr
                // Insert jmp from the source to the inline handler.
                const auto& [old_perm, success] = winapi::set_permission(ptr, original_code_len, winapi::Perm::ExecuteReadWrite);
                write_nop(ptr, original_code_len);
                // Ensure original function has the trampoline area nop'd out.
                write_jmp(ptr, jit_area - (11 + relocated.size() + 5));
                winapi::set_permission(ptr, original_code_len, old_perm);
            }
        };
    };
};