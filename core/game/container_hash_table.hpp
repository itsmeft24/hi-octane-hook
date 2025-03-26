#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include "bind.hpp"
#include "block_allocator.hpp"

typedef void (*HashEnumCallBack)(void* val, void* userData);
typedef int (*HashValueFunction)(void* value);
typedef int(*HashCompareFunction)(void* value1, void* value2);

inline int PointerHashValueFunction(void* key) {
    std::string string = std::to_string(reinterpret_cast<uintptr_t>(key));
    int ii = 0;
    for (const auto& c : string) {
        ii = ii << 1 ^ std::tolower(c);
    }
    return ii;
}

inline int StringHashValueFunction(void* key) {
    int hashValue = 0, i = 0;
    auto str = reinterpret_cast<char*>(key);

    while (str[i] != 0) {
        hashValue += str[i++];
    }

    return hashValue;
}

inline int CharHashValueFunction(void* key) {
    return *reinterpret_cast<std::uint8_t*>(key);
}

inline int PointerHashCompareFunction(void* value1, void* value2) {
    return static_cast<int>(value1 == value2);
}

inline int StringHashCompareFunction(void* value1, void* value2) {
    return (strcmp(reinterpret_cast<char*>(value1), reinterpret_cast<char*>(value2)) == 0) ? 1 : 0;
}

inline int CharHashCompareFunction(void* value1, void* value2) {
    auto value_a = *reinterpret_cast<std::uint8_t*>(value1);
    auto value_b = *reinterpret_cast<std::uint8_t*>(value2);
    return value_a == value_b;
}

template <class TKey, class TVal>
class ContainerHashTable {
public:
    struct Node {
        TVal value;
        int hash; // hash value before modding to buckets size
        TKey key;
        Node* next;
        int bucket_index;
    };

public:

    // the hash buckets itself is stored as an array of linked lists.
    // each linked list is called a "bucket"
    // it helps the hashing if the number of buckets is a prime number
    Node** buckets;
    int bucket_count; // how many buckets
    bool initialized;
    BlockAllocator* block_allocator;
    HashValueFunction    hash_value_func; // The function used to compare keys
    HashCompareFunction  hash_compare_func; // The function used to hash keys

public:
    ContainerHashTable();
    ~ContainerHashTable();
    ContainerHashTable(const ContainerHashTable&) = delete;
    ContainerHashTable& operator=(const ContainerHashTable&) = delete;
    void CHTCreateFull(int _bucket_count, int nodes_per_block, HashValueFunction _hash_value_func, HashCompareFunction _hash_compare_func);
    int Lookup(TKey key, TVal* value);
    int ChangeValue(TKey key, TVal value);
    void CHTAdd(TKey key, TVal value);
    void CHTRemove(TKey key, TKey* retKey = nullptr, TVal* retVal = nullptr);
    void EnumAllNodes(HashEnumCallBack hashEnumCallback, void* userData);
    void RemoveAllNodes();
    void RemoveNode(Node* node);
    
    /*
    TVal GetNextValue(Node** node);
    int GetNextValue(Node** node, TKey* key, TVal* value);
    */
    REPLACE_OPERATOR_NEW_DELETE;
};

template <class TKey, class TVal>
ContainerHashTable<TKey, TVal>::ContainerHashTable() {
    buckets = nullptr;
    bucket_count = 0;
    initialized = false;
    block_allocator = nullptr;
    hash_value_func = nullptr;
    hash_compare_func = nullptr;
}

template<class TKey, class TVal>
inline void ContainerHashTable<TKey, TVal>::CHTCreateFull(int _bucket_count, int nodes_per_block, HashValueFunction _hash_value_func, HashCompareFunction _hash_compare_func)
{
    bucket_count = _bucket_count;
    initialized = true;
    buckets = reinterpret_cast<Node**>(operator_new(sizeof(Node*) * bucket_count));

    for (int i = 0; i < bucket_count; i++) {
        buckets[i] = nullptr;
    }

    hash_value_func = _hash_value_func;
    hash_compare_func = _hash_compare_func;

    block_allocator = new BlockAllocator(sizeof(Node), sizeof(Node) * nodes_per_block);
}

template <class TKey, class TVal>
ContainerHashTable<TKey, TVal>::~ContainerHashTable()
{
    if (initialized) {
        Node* curr, * last;

        // iterate thru buckets
        for (int bucket = 0; bucket < bucket_count; bucket++) {
            // delete all the nodes in this bucket
            curr = buckets[bucket];
            while (curr) {
                last = curr;
                curr = curr->next;
                // operator_delete(last);
                block_allocator->FreeBlock(last);
            }
        }

        operator_delete(buckets);
    }

    if (block_allocator != nullptr) {
        block_allocator->Release();
    }
}

template <class TKey, class TVal>
int ContainerHashTable<TKey, TVal>::Lookup(TKey key, TVal* value)
{
    int origHash = hash_value_func((void*)key);
    int hv = std::abs(origHash) % bucket_count;

    Node* node = buckets[hv];

    while (node) {
        if (node->hash == origHash) {
            if (hash_compare_func((void*)node->key, (void*)key)) {
                *value = node->value;
                return 1;
            }
        }
        node = node->next;
    }
    return 0;
}

template <class TKey, class TVal>
int ContainerHashTable<TKey, TVal>::ChangeValue(TKey key, TVal value)
{
    int origHash = hash_value_func((void*)key);
    int hv = std::abs(origHash) % bucket_count;

    Node* node = buckets[hv];

    while (node) {
        // find a potential match?
        if (node->hash == origHash) {
            if (hash_compare_func((void*)node->key, (void*)key)) {
                node->value = value;
                return 1;
            }
        }
        node = node->next;
    }
    return 0;
}

// doesn't check for duplicate entries
template <class TKey, class TVal>
void ContainerHashTable<TKey, TVal>::CHTAdd(TKey key, TVal value)
{
    int origHash = hash_value_func((void*)key);
    int hv = std::abs(origHash) % bucket_count;

    // add this node to the head of the linked lists of nodes that is bucket[hv]
    // Node* node = reinterpret_cast<Node*>(operator_new(sizeof(Node)));
    int allocated_new_block = 0;
    Node* node = reinterpret_cast<Node*>(block_allocator->AllocBlock(&allocated_new_block));
    node->next = buckets[hv];
    buckets[hv] = node;
    node->key = key;
    node->value = value;
    node->hash = origHash;
    node->bucket_index = hv;
}

template <class TKey, class TVal>
void ContainerHashTable<TKey, TVal>::CHTRemove(TKey key, TKey* retKey /* = nullptr */, TVal* retVal /* = nullptr */)
{
    int origHash = hash_value_func((void*)key);
    int hv = std::abs(origHash) % bucket_count;

    Node* node = buckets[hv];
    Node* prevNode = 0;

    while (node) {
        // find a potential match?
        if (node->hash == origHash) {
            if (hash_compare_func((void*)node->key, (void*)key)) {
                if (prevNode) {
                    prevNode->next = node->next;
                }
                else {
                    buckets[hv] = node->next;
                }
                if (retKey) {
                    *retKey = node->key;
                }
                if (retVal) {
                    *retVal = node->value;
                }

                // operator_delete(node);
                block_allocator->FreeBlock(node);
                return;
            }
        }
        prevNode = node;
        node = node->next;
    }
}

template <class TKey, class TVal>
void ContainerHashTable<TKey, TVal>::EnumAllNodes(HashEnumCallBack hashEnumCallback, void* userData)
{
    Node* node;
    for (int b = 0; b < bucket_count; b++) {
        node = buckets[b];
        if (node) {
            hashEnumCallback(&node->value, userData);
            node = node->next;
        }
        while (node) {
            hashEnumCallback(&node->value, userData);
            node = node->next;
        }
    }
}

template <class TKey, class TVal>
void ContainerHashTable<TKey, TVal>::RemoveAllNodes()
{
    Node* node;
    for (int b = 0; b < bucket_count; b++) {
        node = buckets[b];
        RemoveNode(node);
        buckets[b] = nullptr;
    }
}

template <class TKey, class TVal>
void ContainerHashTable<TKey, TVal>::RemoveNode(Node* node)
{
    if (node) {
        RemoveNode(node->next);
        // operator_delete(node);
        block_allocator->FreeBlock(node);
    }
}

/*
// return the value of the node after the given one
// (node==nullptr to start the search)
template <class TKey, class TVal>
TVal ContainerHashTable<TKey, TVal>::GetNextValue(Node** node)
{
    Node* curr;
    int bucket;

    if (*node == nullptr) {
        for (bucket = 0; bucket < bucket_count; bucket++) {
            curr = buckets[bucket];
            if (curr) {
                (*node) = curr;
                // return the value at this node
                return curr->value;
            }
        }
    }
    else {
        // move to the next node
        curr = (*node);
        int startBucket = curr->bucket_index + 1;
        curr = curr->next;

        if (curr == nullptr) {
            for (bucket = startBucket; bucket < bucket_count; bucket++) {
                curr = buckets[bucket];
                if (curr) {
                    (*node) = curr;
                    // return the value at this node
                    return curr->value;
                }
            }
        }
        else {
            (*node) = curr;
            // return the value at this node
            return curr->value;
        }
    }

    (*node) = nullptr;
    return ((TVal)nullptr);
}

// return the value and key of the node after the given one
// (node==nullptr to start the search)template <class TKey, class TVal>
template <class TKey, class TVal>
int ContainerHashTable<TKey, TVal>::GetNextValue(Node** node, TKey* key, TVal* value)
{
    Node* curr;
    int bucket;

    if (*node == nullptr) {
        for (bucket = 0; bucket < bucket_count; bucket++) {
            curr = buckets[bucket];
            if (curr) {
                (*node) = curr;
                // return the value and key of this node
                *key = curr->key;
                *value = curr->value;
                return 1;
            }
        }
    }
    else {
        // move to the next node
        curr = (*node);
        int startBucket = curr->bucket_index + 1;
        curr = curr->next;

        if (curr == nullptr) {
            for (bucket = startBucket; bucket < bucket_count; bucket++) {
                curr = buckets[bucket];
                if (curr) {
                    (*node) = curr;
                    // return the value and key of this node
                    *key = curr->key;
                    *value = curr->value;
                    return 1;
                }
            }
        }
        else {
            (*node) = curr;
            // return the value and key of this node
            *key = curr->key;
            *value = curr->value;
            return 1;
        }
    }

    (*node) = nullptr;
    return 0;
}
*/

static_assert(sizeof(ContainerHashTable<const char*, int>::Node) == 20);
static_assert(sizeof(ContainerHashTable<const char*, int>) == 24);