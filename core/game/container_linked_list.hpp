#pragma once
#include <cstdint>

template <typename T> struct ContainerLinkedListNode {
	T data;
	ContainerLinkedListNode<T>* prev;
	ContainerLinkedListNode<T>* next;
};

template <typename T> class ContainerLinkedList {
private:
	std::uint32_t len;
	ContainerLinkedListNode<T>* start_node;
	ContainerLinkedListNode<T>* end_node;
public:
	template <typename T> class Iterator {
	private:
		ContainerLinkedListNode<T>* current;
	public:
		Iterator(ContainerLinkedListNode<T>* node) : current(node) {}

		constexpr T& operator*() const {
			return current->data;
		}

		constexpr Iterator& operator++() {
			if (current != nullptr) {
				current = current->next;
			}
			return *this;
		}

		constexpr Iterator& operator--() {
			if (current != nullptr) {
				current = current->prev;
			}
			return *this;
		}

		constexpr bool operator==(const Iterator& other) const {
			return current == other.current;
		}

		constexpr bool operator!=(const Iterator& other) const {
			return current != other.current;
		}
	};

	ContainerLinkedList(const ContainerLinkedList<T>&) = delete;
	ContainerLinkedList& operator=(const ContainerLinkedList<T>&) = delete;
	
	inline ContainerLinkedList() {
		len = 0;
		start_node = nullptr;
		end_node = nullptr;
	}

	inline ~ContainerLinkedList() {
		if (this != nullptr) {
			auto node = start_node;
			while (node != nullptr) {
				auto next_node = node->next;
				operator_delete(node);
				node = next_node;
			}
			len = 0;
			start_node = nullptr;
			end_node = nullptr;
		}
	}
	
	inline int Append(T elem) {
		auto new_node = reinterpret_cast<ContainerLinkedList<T>*>(operator_new(sizeof(ContainerLinkedList<T>)));
		if (new_node == nullptr) {
			return 0;
		}
		new_node->prev = nullptr;
		new_node->next = nullptr;
		new_node->data = elem;
		new_node->prev = end_node;
		new_node->next = nullptr;
		if (end_node == nullptr) {
			start_node = new_node;
			end_node = new_node;
		}
		else {
			end_node->next = new_node;
			end_node = new_node;
		}
		len++;
		return 1;
	}
	
	inline int AppendUnique(T elem) {
		auto node = start_node;
		while (node != nullptr) {
			if (node->data == elem) {
				return 0;
			}
			node = node->next;
		}
		return Append(elem);
	}
	
	inline int PreInsert(ContainerLinkedListNode<T>* pre_node, T elem) {
		auto new_node = reinterpret_cast<ContainerLinkedList<T>*>(operator_new(sizeof(ContainerLinkedList<T>)));
		if (new_node == nullptr) {
			return 0;
		}
		new_node->prev = nullptr;
		new_node->next = nullptr;
		new_node->data = elem;
		new_node->prev = pre_node->prev;
		new_node->next = pre_node;
		if (pre_node->prev != nullptr) {
			pre_node->prev->next = new_node;
		}
		pre_node->prev = new_node;
		if (pre_node == start_node) {
			start_node = new_node;
		}
		len++;
		return 1;
	}

	inline std::uint32_t GetNumElements() const {
		return len;
	}
	
	inline ContainerLinkedListNode<T>* GetListNode(T elem) const {
		auto node = start_node;
		while (node != nullptr) {
			if (node->data == elem) {
				return node;
			}
			node = node->next;
		}
		return nullptr;
	}
	
	inline ContainerLinkedListNode<T>* GetNthNode(int index) const {
		if (index >= len) {
			return nullptr;
		}
		auto node = start_node;
		int i = index;
		while (node != nullptr && i != 0) {
			node = node->next;
			i--;
		}
		return node;
	}
	
	inline T GetNthElement(int index) const {
		if (auto node = GetNthNode(index); node != nullptr) {
			return node->data;
		}
	}
	
	inline T operator[](int index) const {
		return GetNthElement(index);
	}
	
	// why does this return T???
	inline T ElementInList(T elem) const {
		auto node = start_node;
		while (node != nullptr) {
			if (node->data == elem) {
				return elem;
			}
			node = node->next;
		}
		return T{};
	}

	inline void RemoveNode(ContainerLinkedListNode<T>* node) {
		if (node == start_node) {
			start_node = node->next;
		}
		if (node == end_node) {
			end_node = node->prev;
		}
		if (node->prev != nullptr) {
			node->prev->next = node->next;
		}
		if (node->next != nullptr) {
			node->next->prev = node->prev;
		}
		if (node != nullptr) {
			operator_delete(node);
		}
		len--;
	}
	
	inline T RemoveNthElement(int index) {
		auto node = GetNthNode(index);
		if (node != nullptr) {
			auto data = node->data;
			RemoveNode(node);
			return data;
		}
		return T{};
	}
	
	inline T RemoveElement(T elem) {
		auto node = GetListNode(elem);
		if (node != nullptr) {
			auto data = node->data;
			RemoveNode(node);
			return data;
		}
		return T{};
	}

	inline void EmptyList() {
		auto node = start_node;
		while (node != nullptr) {
			auto next_node = node->next;
			operator_delete(node);
			node = next_node;
		}
		len = 0;
		start_node = nullptr;
		end_node = nullptr;
	}

	Iterator<T> begin() const {
		return Iterator<T>(start_node);
	}

	Iterator<T> end() const {
		return Iterator<T>(end_node);
	}
};

template <class T> using LinkedList = ContainerLinkedList<T>;
template <class T> using LinkedListNode = ContainerLinkedListNode<T>;