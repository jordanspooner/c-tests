#include "binaryheap.h"

node_heap *allocate_node_heap(void) {
	node_heap *node = malloc(sizeof(node_heap));
	if (node == NULL) {
		perror("Call to malloc in allocate_node_heap failed");
		exit(EXIT_FAILURE);
	}
	node->key = "";
	node->position = 0;
	return node;
}

void initial_heap(node_heap **heap, char *sequence) {
	assert(heap != NULL && sequence != NULL);
	for (int i = 1; i <= strlen(sequence); i++) {
		node_heap *node = allocate_node_heap();
		node->key = &sequence[i-1];
		node->position = i;
		heap[i] = node;
	}
}

void print_elem_heap(node_heap **heap, int length) {
	assert(heap != NULL);
	for (int i = 1; i <= length; i++) {
		assert(heap[i] != NULL);
		printf("%c", *heap[i]->key);
	}
	printf("\n");
	for (int i = 1; i<= length; i++) {
		printf("%d", heap[i]->position);
	}
	printf("\n");
}

int parent(int index) {
	return index / 2;
}

int left_child(int index) {
	return 2 * index;
}

int right_child(int index) {
	return 2 * index + 1;
}

void swap(node_heap *node1, node_heap *node2) {
	assert(node1 != NULL && node2 != NULL);
	node_heap temp = *node1;
	*node1 = *node2;
	*node2 = temp;
}

void max_heapify(node_heap **heap, int current, int heap_size) {
	assert(heap != NULL && heap[current] != NULL);
	// Only if this node has a child
	if (left_child(current) <= heap_size) {
		// Find the maximum child
		int max_child = 0;
		if (right_child(current) <= heap_size) {
			if (*heap[left_child(current)]->key >= *heap[right_child(current)]->key) {
				max_child = left_child(current);
			} else {
				max_child = right_child(current);
			}
		} else {
			max_child = left_child(current);
		}
		// If the max child is bigger than current, swap values and sift recursively
		if (*heap[max_child]->key > *heap[current]->key) {
			swap(heap[current], heap[max_child]);
			max_heapify(heap, max_child, heap_size);
		}
	}
}

void build_max_heap(node_heap **heap, int heap_size) {
	assert(heap != NULL);
	for (int i = heap_size / 2; i > 0; i--) {
		max_heapify(heap, i, heap_size);
	}
}

void heapsort(node_heap **heap, int length) {
	assert(heap != NULL);
	while (length > 1) {
		swap(heap[1], heap[length]);
		length--;
		max_heapify(heap, 1, length);
	}
}

void free_node(node_heap *node) {
	if (node != NULL) {
		free(node);
	}
}

void free_heap(node_heap **heap, int length) {
	assert (heap != NULL);
	for (int i = 1; i <= length; i++) {
		free_node(heap[i]);
	}
}
