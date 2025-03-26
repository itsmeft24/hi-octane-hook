#pragma once
#include "core/game/bind.hpp"
#include <cstddef>

template <typename T> class UILinkedListNode {
public:
	T value;
	UILinkedListNode<T>* prev;
	UILinkedListNode<T>* next;
public:
	virtual ~UILinkedListNode<T>() {
	}

	REPLACE_OPERATOR_NEW_DELETE
};

template <typename T> class UILinkedList {
private:
	UILinkedListNode<T>* start;
	std::size_t len;
public:
	virtual ~UILinkedList<T>() {
		UILinkedListNode<T>* node = this->start;
		while (len > 0) {
			if (node == nullptr) {
				break;
			}
			UILinkedListNode<T>* next_node = node->next;
			node->~UILinkedListNode<T>();
			node = next_node;
			len--;
		}
	}

	REPLACE_OPERATOR_NEW_DELETE
};

static_assert(sizeof(UILinkedListNode<int>) == 0x10);
static_assert(sizeof(UILinkedList<int>) == 0xc);