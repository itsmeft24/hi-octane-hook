#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include "bind.hpp"

typedef void (*HashEnumCallBack)(void* val, void* userData);
typedef int (*HashValueFunction)(void* value);
typedef int(*HashCompareFunction)(void* value1, void* value2);

// Generates hash values based on the address referenced by key
inline int PointerHashValueFunction(void* key) {
    std::string string = std::to_string(reinterpret_cast<uintptr_t>(key));
    int ii = 0;
    for (const auto& c : string) {
        ii = ii << 1 ^ std::tolower(c);
    }
    return ii;
}

// Generates hash values based on the string referenced by key
inline int StringHashValueFunction(void* key) {
    int hashValue = 0, i = 0;
    auto str = reinterpret_cast<char*>(key);

    while (str[i] != 0) {
        hashValue += str[i++];
    }

    return hashValue;
}

// Generates hash value as if the key was a single character
inline int CharHashValueFunction(void* key) {
    return *reinterpret_cast<std::uint8_t*>(key);
}

// Compares the address referenced by value1 and value2
inline int PointerHashCompareFunction(void* value1, void* value2) {
    return static_cast<int>(value1 == value2);
}

// Compares the strings referenced by value1 and value2
inline int StringHashCompareFunction(void* value1, void* value2) {
    return (strcmp(reinterpret_cast<char*>(value1), reinterpret_cast<char*>(value2)) == 0) ? 1 : 0;
}

inline int CharHashCompareFunction(void* value1, void* value2) {
    auto valuea = *reinterpret_cast<std::uint8_t*>(value1);
    auto valueb = *reinterpret_cast<std::uint8_t*>(value2);
    return valuea == valueb;
}

template <class TKey, class TVal>
class ContainerHashTable {
public:
    struct Node
    {
        TVal value;
        int hash; // hash value before modding to buckets size
        TKey key;
        Node* next_node;
        int bucket_index;
    };

public:

    // the hash buckets itself is stored as an array of linked lists.
    // each linked list is called a "bucket"
    // it helps the hashing if the number of buckets is a prime number
    Node** buckets;
    int bucket_count; // how many buckets
    void* unk;
    struct BlockAllocator* alloc;
    HashValueFunction    hash_value_func; // The function used to compare keys
    HashCompareFunction  hash_compare_func; // The function used to hash keys

public:
    ContainerHashTable(int numBuckets);
    ContainerHashTable(int numBuckets, HashValueFunction hashValueFunction, HashCompareFunction hashCompareFunction);
    ~ContainerHashTable();
    ContainerHashTable(const ContainerHashTable&) = delete;
    ContainerHashTable& operator=(const ContainerHashTable&) = delete;

    // find the value for this key
    int Lookup(TKey key, TVal* value);

    // change the value for this key
    // DOESN'T check for duplicate entries
    int ChangeValue(TKey key, TVal value);

    // add a key/value pair
    // DOESN'T check for duplicate entries
    void Add(TKey key, TVal value);

    // remove a key/value pair
    // DOESN'T check for duplicate entries
    void Remove(TKey key, TKey* retKey = NULL, TVal* retVal = NULL);

    // call function on all nodes
    void EnumAllNodes(HashEnumCallBack hashEnumCallback, void* userData);

    // remove all nodes in the hash table (i.e. return it to its start state)
    void RemoveAllNodes();

    // recursively remove all nodes in node's list
    void RemoveNode(Node* node);

    // return the value of the node after the given one
    // (node==NULL to start the search)
    TVal GetNextValue(Node** node);

    // return the value and key of the node after the given one
    // (node==NULL to start the search)
    int GetNextValue(Node** node, TKey* key, TVal* value);
};

template <class TKey, class TVal>
ContainerHashTable<TKey, TVal>::ContainerHashTable(int numBuckets)
{
    bucket_count = numBuckets;
    buckets = reinterpret_cast<Node**>(operator_new(sizeof(Node*) * bucket_count));

    for (int i = 0; i < bucket_count; i++) {
        buckets[i] = NULL;
    }

    hash_value_func = PointerHashValueFunction;
    hash_compare_func = PointerHashCompareFunction;
}

template <class TKey, class TVal>
ContainerHashTable<TKey, TVal>::ContainerHashTable(int numBuckets, HashValueFunction hashValueFunction, HashCompareFunction hashCompareFunction)
{
    bucket_count = numBuckets;
    buckets = reinterpret_cast<Node**>(operator_new(sizeof(Node*) * bucket_count));

    for (int i = 0; i < bucket_count; i++) {
        buckets[i] = NULL;
    }

    hash_value_func = hashValueFunction;
    hash_compare_func = hashCompareFunction;
}

template <class TKey, class TVal>
ContainerHashTable<TKey, TVal>::~ContainerHashTable()
{
    int bucket;
    Node* curr, * last;

    // iterate thru buckets
    for (bucket = 0; bucket < bucket_count; bucket++) {
        // delete all the nodes in this bucket
        curr = buckets[bucket];
        while (curr) {
            last = curr;
            curr = curr->Next;
            operator_delete(last);
        }
    }

    operator_delete(buckets);
}

// find the value for this key
template <class TKey, class TVal>
int ContainerHashTable<TKey, TVal>::Lookup(TKey key, TVal* value)
{
    int origHash = hash_value_func((void*)key);
    int hv = ABS(origHash) % bucket_count;

    Node* node = buckets[hv];

    while (node) {
        // find a potential match?
        if (node->OrigHash == origHash) {
            if (hash_compare_func((void*)node->Key, (void*)key)) {
                *value = node->Val;
                return 1;
            }
        }
        node = node->Next;
    }
    return 0;
}

// change the value for this key
template <class TKey, class TVal>
int ContainerHashTable<TKey, TVal>::ChangeValue(TKey key, TVal value)
{
    int origHash = hash_value_func((void*)key);
    int hv = ABS(origHash) % bucket_count;

    Node* node = buckets[hv];

    while (node) {
        // find a potential match?
        if (node->OrigHash == origHash) {
            if (hash_compare_func((void*)node->Key, (void*)key)) {
                node->Val = value;
                return 1;
            }
        }
        node = node->Next;
    }
    return 0;
}

// add a key/value pair
// DOESN'T check for duplicate entries
template <class TKey, class TVal>
void ContainerHashTable<TKey, TVal>::Add(TKey key, TVal value)
{
    int origHash = hash_value_func((void*)key);
    int hv = ABS(origHash) % bucket_count;

    // add this node to the head of the linked lists of nodes that is bucket[hv]
    Node* node = reinterpret_cast<Node*>(operator_new(sizeof(Node)));
    node->Next = buckets[hv];
    buckets[hv] = node;
    node->Key = key;
    node->Val = value;
    node->OrigHash = origHash;
    node->Bucket = hv;
}

// find the value for this key
template <class TKey, class TVal>
void ContainerHashTable<TKey, TVal>::Remove(TKey key, TKey* retKey /* = NULL */, TVal* retVal /* = NULL */)
{
    int origHash = hash_value_func((void*)key);
    int hv = ABS(origHash) % bucket_count;

    Node* node = buckets[hv];
    Node* prevNode = 0;

    while (node) {
        // find a potential match?
        if (node->OrigHash == origHash) {
            if (hash_compare_func((void*)node->Key, (void*)key)) {
                if (prevNode) {
                    prevNode->Next = node->Next;
                }
                else {
                    buckets[hv] = node->Next;
                }
                if (retKey) {
                    *retKey = node->Key;
                }
                if (retVal) {
                    *retVal = node->Val;
                }

                operator_delete(node);
                return;
            }
        }
        prevNode = node;
        node = node->Next;
    }
}

// call function on all nodes
template <class TKey, class TVal>
void ContainerHashTable<TKey, TVal>::EnumAllNodes(HashEnumCallBack hashEnumCallback, void* userData)
{
    Node* node;
    for (int b = 0; b < bucket_count; b++) {
        node = buckets[b];
        if (node) {
            hashEnumCallback(&node->Val, userData);
            node = node->Next;
        }
        while (node) {
            hashEnumCallback(&node->Val, userData);
            node = node->Next;
        }
    }
}

// remove all nodes in the hash table (i.e. return it to its start state)
template <class TKey, class TVal>
void ContainerHashTable<TKey, TVal>::RemoveAllNodes()
{
    Node* node;
    for (int b = 0; b < bucket_count; b++) {
        node = buckets[b];
        RemoveNode(node);
        buckets[b] = NULL;
    }
}

// recursivley remove all nodes in node's list
template <class TKey, class TVal>
void ContainerHashTable<TKey, TVal>::RemoveNode(Node* node)
{
    if (node) {
        RemoveNode(node->Next);
        operator_delete(node);
    }
}

// return the value of the node after the given one
// (node==NULL to start the search)
template <class TKey, class TVal>
TVal ContainerHashTable<TKey, TVal>::GetNextValue(Node** node)
{
    Node* curr;
    int bucket;

    if (*node == NULL) {
        for (bucket = 0; bucket < bucket_count; bucket++) {
            curr = buckets[bucket];
            if (curr) {
                (*node) = curr;
                // return the value at this node
                return curr->Val;
            }
        }
    }
    else {
        // move to the next node
        curr = (*node);
        int startBucket = curr->Bucket + 1;
        curr = curr->Next;

        if (curr == NULL) {
            for (bucket = startBucket; bucket < bucket_count; bucket++) {
                curr = buckets[bucket];
                if (curr) {
                    (*node) = curr;
                    // return the value at this node
                    return curr->Val;
                }
            }
        }
        else {
            (*node) = curr;
            // return the value at this node
            return curr->Val;
        }
    }

    (*node) = NULL;
    return ((TVal)NULL);
}

// return the value and key of the node after the given one
// (node==NULL to start the search)template <class TKey, class TVal>
template <class TKey, class TVal>
int ContainerHashTable<TKey, TVal>::GetNextValue(Node** node, TKey* key, TVal* value)
{
    Node* curr;
    int bucket;

    if (*node == NULL) {
        for (bucket = 0; bucket < bucket_count; bucket++) {
            curr = buckets[bucket];
            if (curr) {
                (*node) = curr;
                // return the value and key of this node
                *key = curr->Key;
                *value = curr->Val;
                return 1;
            }
        }
    }
    else {
        // move to the next node
        curr = (*node);
        int startBucket = curr->Bucket + 1;
        curr = curr->Next;

        if (curr == NULL) {
            for (bucket = startBucket; bucket < bucket_count; bucket++) {
                curr = buckets[bucket];
                if (curr) {
                    (*node) = curr;
                    // return the value and key of this node
                    *key = curr->Key;
                    *value = curr->Val;
                    return 1;
                }
            }
        }
        else {
            (*node) = curr;
            // return the value and key of this node
            *key = curr->Key;
            *value = curr->Val;
            return 1;
        }
    }

    (*node) = NULL;
    return 0;
}
