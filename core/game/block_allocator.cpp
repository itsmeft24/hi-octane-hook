#include "core/globals.hpp"
#include "block_allocator.hpp"

DeclareFunction(void*, __thiscall, _BlockAllocator_AllocBlock_Impl, 0x005a4d10, BlockAllocator*, int*, char*, int);
DeclareFunction(int, __thiscall, _BlockAllocator_AllocBigBlock_Impl, 0x0053dc00, BlockAllocator*, char*, int);
DeclareFunction(void*, __thiscall, _BlockAllocator_AllocBlockComplex_Impl, 0x00574f20, BlockAllocator*, char*, int);

BlockAllocator::BlockAllocator(int _used_size_per_block, int _block_size_bytes) : BaseObject() {
    used_size_per_block = _used_size_per_block;
    alignment_unused = 0;
    unk_unused = 0;
    big_blocks = nullptr;
    last_block = nullptr;
    free_block = nullptr;
    small_blocks = nullptr;
    unk_callback = nullptr;
    block_size_bytes = _block_size_bytes;
    block_data_offset = sizeof(Block*);
    max_extra_uses_per_block = (_block_size_bytes - _used_size_per_block) / _used_size_per_block;
    uses_in_current_block = (_block_size_bytes - _used_size_per_block) / _used_size_per_block;
    free_memory_not_new_block = nullptr;
}

BlockAllocator::~BlockAllocator() {
    Block* block = big_blocks;
    while (block != nullptr) {
        Block* next_block = block->next;
        operator_delete(block);
        block = next_block;
    }

    block = small_blocks;
    while (block != nullptr) {
        Block* next_block = block->next;
        operator_delete(block);
        block = next_block;
    }

    big_blocks = nullptr;
    last_block = nullptr;
    free_block = nullptr;
    small_blocks = nullptr;
    uses_in_current_block = this->max_extra_uses_per_block;
}

void* BlockAllocator::AllocBlock(int* allocated_new_block) {
    return _BlockAllocator_AllocBlock_Impl(this, allocated_new_block, nullptr, 0);
}

int BlockAllocator::AllocBigBlock() {
    return _BlockAllocator_AllocBigBlock_Impl(this, nullptr, 0);
}

void* BlockAllocator::AllocBlockComplex() {
    return _BlockAllocator_AllocBlockComplex_Impl(this, nullptr, 0);
}

void BlockAllocator::FreeBlock(void* memory)
{
    if (4 < used_size_per_block) {
        // WHAT IN THE UNSAFE IS THIS?!?! AAAAAAHHHHHHH
        auto* block = reinterpret_cast<Block*>(memory);
        block->next = free_block;
        free_block = block;
    }
}
