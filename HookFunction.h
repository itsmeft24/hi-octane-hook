#pragma once

enum class FunctionHookType : int {
  Invalid = -1,
  EntireReplacement, // Replace an entire function with your own implementation,
                     // with intent to be able to call the original at any time.
  InlineReplacement, // Replace a segment of code within a function with your
                     // own code.
  IATReplacement,    // Replace an entry in the IAT of the running executable,
                     // provided its address pointer.
  InlineReplacementJMP, // An InlineReplacement that does not use a function
                        // call, instead relying on the user to properly JMP
                        // back to the target function.
};

struct HookedFunctionInfo {
  void *source_address;
  void *destination_address;
  void *original_code;
  FunctionHookType type;
  size_t code_size;

  static constexpr inline HookedFunctionInfo Default() {
    return {(void *)(0xFFFFFFFF), (void *)(0xFFFFFFFF), (void *)(0xFFFFFFFF),
            FunctionHookType::Invalid, 0xFFFFFFFF};
  }
};

void RelocateCodeSegment(void *src, void *dst, const size_t size);

HookedFunctionInfo HookFunction(void *&src, void *dst, const size_t code_size,
                                FunctionHookType type);

HookedFunctionInfo HookFunction(unsigned long src, void *dst,
                                const size_t code_size, FunctionHookType type);

bool UninstallFunctionHook(HookedFunctionInfo &function_info);
