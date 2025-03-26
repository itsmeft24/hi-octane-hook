#pragma once
#include "base_object.hpp"

class BlockAllocator : public BaseObject {
public:
	struct Block {
		Block* next;
	};
private:
	int max_extra_uses_per_block;
	Block* big_blocks;
	Block* last_block;
	Block* small_blocks;
	int uses_in_current_block;
	int used_size_per_block;
	int block_size_bytes;
	Block* free_block;
	void* free_memory_not_new_block;
	void (**unk_callback)(void*);
	// on pc, the game will always call _malloc no matter what; on other platforms it will call memalign if the alignment is set.
	int alignment_unused;
	int unk_unused;
	// always equal to sizeof(Block*), or just 4
	int block_data_offset;
public:
	BlockAllocator(int _used_size_per_block, int _block_size_bytes);
	virtual ~BlockAllocator();

	void* AllocBlock(int* allocated_new_block);
	int AllocBigBlock();
	void* AllocBlockComplex();
	void FreeBlock(void* block);

	REPLACE_OPERATOR_NEW_DELETE;
};

static_assert(sizeof(BlockAllocator::Block) == 4);
static_assert(sizeof(BlockAllocator) == 60);