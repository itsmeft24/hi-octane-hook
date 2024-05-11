#pragma once

template <typename T> struct ContainerLinkedListNode {
	T data;
	ContainerLinkedListNode<T>* prev;
	ContainerLinkedListNode<T>* next;
};

template <typename T> class ContainerLinkedList {
private:
	std::uint32_t len;
	ContainerLinkedListNode<T>* start;
	ContainerLinkedListNode<T>* end;
public:
};