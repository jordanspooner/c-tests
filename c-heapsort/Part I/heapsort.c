#include "binaryheap.h"

enum {MAX_STRING_LENGTH = 20};

int main(int argc, char **argv) {
	assert(argc == 2);

	char *sequence = argv[1];
	int length = strlen(argv[1]);

	assert(length <= MAX_STRING_LENGTH);
	node_heap *heap[MAX_STRING_LENGTH + 1];

	initial_heap(heap, sequence);
	print_elem_heap(heap, length);

	build_max_heap(heap, length);
	print_elem_heap(heap, length);

	heapsort(heap, length);
	print_elem_heap(heap, length);

	free_heap(heap, length);
}