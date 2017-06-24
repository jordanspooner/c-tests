#ifndef BINARYHEAP_H
#define BINARYHEAP_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_heap_t node_heap;

struct node_heap_t{
	char *key;
	int position;
};

node_heap *allocate_node_heap(void);
void initial_heap(node_heap **heap, char *sequence);
void print_elem_heap(node_heap **heap, int length);
int parent(int index);
int left_child(int index);
int right_child(int index);
void swap(node_heap *node1, node_heap *node2);
void max_heapify(node_heap **heap, int current, int heap_size);
void build_max_heap(node_heap **heap, int heap_size);
void heapsort(node_heap **heap, int length);
void free_node(node_heap *node);
void free_heap(node_heap **heap, int length);

#endif