#pragma once
#include <iostream>
#include <cstdint>

template <typename T>
class ContainerList {
private:
	std::size_t length;
	std::uint8_t flags;
	T* memory;
	std::size_t capacity_add;
	std::size_t capacity;
	std::size_t max;
public:
	template <typename T>
	class Iterator {
	private:
		T* current;
	public:
		Iterator(T* ptr) : current(ptr) {}

		constexpr T& operator*() const {
			return *current;
		}

		constexpr Iterator& operator++() {
			++current;
			return *this;
		}

		constexpr Iterator& operator--() {
			--current;
			return *this;
		}

		constexpr bool operator==(const Iterator& other) const {
			return current == other.current;
		}

		constexpr bool operator!=(const Iterator& other) const {
			return current != other.current;
		}
	};

	ContainerList(const ContainerList&) = delete;
	ContainerList& operator=(const ContainerList&) = delete;

	inline ContainerList() {
		memory = nullptr;
		length = 0;
		capacity_add = 0;
		capacity = 0;
		max = 0;
		flags = flags | 1;
	}

	inline ~ContainerList() {
		if (this != nullptr) {
			if (memory != nullptr) {
				free(memory);
			}
		}
	}

	inline int CLAddItem(T* item) {
		if (length < capacity || CLGrow(length + capacity_add) != 0) {
			memory[length] = item;
			length++;
			return 1;
		}
		else {
			return 0;
		}
	}

	inline void Clear() {
		if (flags < 0 && memory != nullptr) {
			std::memset(memory, 0, length * sizeof(T));
			length = 0;
		}
		return;
	}

	inline int CLGrow(std::size_t new_capacity) {
		if (capacity < new_capacity) {
			if (max < new_capacity) {
				return 0;
			}
			else {
				memory = realloc(memory, sizeof(T) * new_capacity);
				capacity = new_capacity;
				return 1;
			}
		}
		else {
			return 0;
		}
	}

	inline std::size_t CLNonMacroCreate(int initial_capacity, int capacity_growth, int max_capacity) {
		length = 0;
		memory = malloc(initial_capacity * sizeof(T));
		capacity_add = capacity_growth;
		capacity = initial_capacity;
		max = max_capacity;
		flags = flags | 0x80;
		return 0;
	}

	inline T GetItem(std::size_t index) {
		return memory[index];
	}

	inline std::size_t RemoveItem(T value) {
		return 0;
	}

	inline std::size_t Length() {
		return length;
	}

	inline T& operator[](std::size_t index) {
		return memory[index];
	}
	
	Iterator<T> begin() const {
		return Iterator<T>(memory);
	}

	Iterator<T> end() const {
		return Iterator<T>(memory + length);
	}
};