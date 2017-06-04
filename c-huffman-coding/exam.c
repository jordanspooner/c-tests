#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exam.h"

/*
 * Private function prototypes.
 */

static void _print_huffman_tree(const huffman_tree_t *, int);
static void _print_huffman_tree_codes(const huffman_tree_t *, char *, char *);

/*
 * Prints the given Huffman tree.
 */
void print_huffman_tree(const huffman_tree_t *t) {
  printf("Huffman tree:\n");
  _print_huffman_tree(t, 0);
}

/*
 * Private helper function for print_huffman_tree.
 */
static void _print_huffman_tree(const huffman_tree_t *t, int level) {
  int i;
  for (i = 0; i <= level; i++) {
    printf("  ");
  }

  if (t->left == NULL && t->right == NULL) {
    printf("Leaf: '%c' with count %d\n", t->letter, t->count);
  } else {
    printf("Node: accumulated count %d\n", t->count);

    if (t->left != NULL) {
      _print_huffman_tree(t->left, level + 1);
    }

    if (t->right != NULL) {
      _print_huffman_tree(t->right, level + 1);
    }
  }
}

/*
 * Prints the codes contained in the given Huffman tree.
 */
void print_huffman_tree_codes(const huffman_tree_t *t) {
  printf("Huffman tree codes:\n");

  char *code = calloc(MAX_CODE_LENGTH, sizeof(char)), *code_position = code;
  if (code == NULL) {
    perror("calloc");
    exit(EXIT_FAILURE);
  }

  _print_huffman_tree_codes(t, code, code_position);
  free(code);
}

/*
 * Private helper function for print_huffman_tree_codes.
 */
static void _print_huffman_tree_codes(const huffman_tree_t *t,
                                      char *code, char *code_position) {

  if (t->left == NULL && t->right == NULL) {
    printf("'%c' has code \"%s\"\n", t->letter, code);
  } else if (t->left != NULL) {
    *code_position++ = 'L';
    *code_position = '\0';

    _print_huffman_tree_codes(t->left, code, code_position--);
  }

  if (t->right != NULL) {
    *code_position++ = 'R';
    *code_position = '\0';

    _print_huffman_tree_codes(t->right, code, code_position--);
  }
}

/*
 * Prints a list of Huffman trees.
 */
void print_huffman_tree_list(const huffman_tree_list_t *l) {
  printf("Huffman tree list:\n");

  for (; l != NULL; l = l->next) {
    print_huffman_tree(l->tree);
  }
}

/*
 * Frees a Huffman tree.
 */
void huffman_tree_free(huffman_tree_t *t) {
  if (t != NULL) {
    huffman_tree_free(t->left);
    huffman_tree_free(t->right);
    free(t);
  }
}

/*
 * Frees a list of Huffman trees.
 */
void huffman_tree_list_free(huffman_tree_list_t *l) {
  if (l != NULL) {
    huffman_tree_list_free(l->next);
    huffman_tree_free(l->tree);
    free(l);
  }
}

/*
 * Returns 1 if the string s contains the character c and 0 if it does not.
 */
int contains(char *s, char c) {
  if (frequency(s, c) > 0) {
    return 1;
  }
  return 0;
}

/*
 * Returns the number of occurrences of c in s.
 */
int frequency(char *s, char c) {
  assert(s != NULL);
  int count = 0;
  for (int i = 0; s[i] != '\0'; i++) {
    if (s[i] == c) {
      count++;
    }
  }
  return count;
}

/*
 * Takes a string s and returns a new heap-allocated string containing only the
 * unique characters of s.
 *
 * Pre: all strings will have fewer than or equal to MAX_STRING_LENGTH - 1
 *      characters.
 */
char *nub(char *s) {
  assert(s != NULL);
  char *uniques = calloc(strlen(s) + 1, sizeof(char));
  if (uniques == NULL) {
    perror("nub");
    exit(EXIT_FAILURE);
  }
  int uniques_count = 0;
  for (int i = 0; s[i] != '\0'; i++) {
    if (!contains(uniques, s[i])) {
      uniques[uniques_count] = s[i];
      uniques_count++;
    }
  }
  return uniques;
}

/*
 * Adds the Huffman tree t to the list l, returning a pointer to the new list.
 *
 * Pre:   The list l is sorted according to the frequency counts of the trees
 *        it contains.
 *
 * Post:  The list l is sorted according to the frequency counts of the trees
 *        it contains.
 */
huffman_tree_list_t *huffman_tree_list_add(huffman_tree_list_t *l,
                                            huffman_tree_t *t) {
  assert(t != NULL);
  // Check if at end of list
  if (l == NULL) {
    huffman_tree_list_t *new_list = malloc(sizeof(huffman_tree_list_t));
    if (new_list == NULL) {
      perror("huffman_tree_list_add");
      exit(EXIT_FAILURE);
    }
    new_list->tree = t;
    new_list->next = NULL;
    return new_list;
  // Insert after this element
  } else if (t->count > l->tree->count) {
    l->next = huffman_tree_list_add(l->next, t);
    return l;
  // Insert here
  } else {
    huffman_tree_list_t *new_list = malloc(sizeof(huffman_tree_list_t));
    if (new_list == NULL) {
      perror("huffman_tree_list_add");
      exit(EXIT_FAILURE);
    }
    new_list->tree = t;
    new_list->next = l;
    return new_list;
  }
}

/*
 * Takes a string s and a lookup table and builds a list of Huffman trees
 * containing leaf nodes for the characters contained in the lookup table. The
 * leaf nodes' frequency counts are derived from the string s.
 *
 * Pre:   t is a duplicate-free version of s.
 *
 * Post:  The resulting list is sorted according to the frequency counts of the
 *        trees it contains.
 */
huffman_tree_list_t *huffman_tree_list_build(char *s, char *t) {
  assert(s != NULL);
  assert(t != NULL);
  huffman_tree_list_t *l = NULL;
  for (int i = 0; t[i] != 0; i++) {
    huffman_tree_t *tree = malloc(sizeof(huffman_tree_t));
    if (tree == NULL) {
      perror("huffman_tree_list_build");
      exit(EXIT_FAILURE);
    }
    tree->count = frequency(s, t[i]);
    tree->letter = t[i];
    tree->left = NULL;
    tree->right = NULL;
    l = huffman_tree_list_add(l, tree);
  }
  return l;
}

/*
 * Reduces a sorted list of Huffman trees to a single element.
 *
 * Pre:   The list l is non-empty and sorted according to the frequency counts
 *        of the trees it contains.
 *
 * Post:  The resuling list contains a single, correctly-formed Huffman tree.
 */
huffman_tree_list_t *huffman_tree_list_reduce(huffman_tree_list_t *l) {
  assert(l != NULL);
  if (l->next != NULL) {
    huffman_tree_t *tree = malloc(sizeof(huffman_tree_t));
    if (tree == NULL) {
      perror("huffman_tree_list_reduce");
      exit(EXIT_FAILURE);
    }
    huffman_tree_t *left = l->tree;
    huffman_tree_t *right = l->next->tree;
    huffman_tree_list_t *rest = l->next->next;

    free(l->next);
    free(l);

    tree->count = left->count + right->count;
    tree->letter = 0;
    tree->left = left;
    tree->right = right;

    return huffman_tree_list_reduce(huffman_tree_list_add(rest, tree));
  }
  return l;
}

static int huffman_tree_contains(huffman_tree_t *t, char c) {
  if (t == NULL) {
    return 0;
  } else if (t->letter == c) {
    return 1;
  } else {
    return huffman_tree_contains(t->left, c)
        || huffman_tree_contains(t->right, c);
  }
}

/*
 * Accepts a Huffman tree t and a string s and returns a new heap-allocated
 * string containing the encoding of s as per the tree t.
 *
 * Pre: s only contains characters present in the tree t.
 */
char *huffman_tree_encode(huffman_tree_t *t, char *s) {
  char *return_string = malloc(MAX_CODE_LENGTH * sizeof(char));
  if (return_string == NULL) {
    perror("huffman_tree_encode");
    exit(EXIT_FAILURE);
  }
  return_string[0] = '\0';

  for (int i = 0; s[i] != '\0'; i++) {
    huffman_tree_t *current_node = t;

    while (current_node->letter != s[i]) {
      if (huffman_tree_contains(current_node->left, s[i])) {
        strcat(return_string, "L");
        current_node = current_node->left;
      } else {
        strcat(return_string, "R");
        current_node = current_node->right;
      }

      if (current_node == NULL) {
        fprintf(stderr, "Character (%c) could not be found. Exiting.\n", s[i]);
        exit(EXIT_FAILURE);
      }
    }
  }

  return return_string;
}

/*
 * Accepts a Huffman tree t and an encoded string and returns a new
 * heap-allocated string contained the decoding of the code as per the tree t.
 *
 * Pre: the code given is decodable using the supplied tree t.
 */
char *huffman_tree_decode(huffman_tree_t *t, char *code) {
  char *return_string = malloc(MAX_STRING_LENGTH * sizeof(char));
  if (return_string == NULL) {
    perror("huffman_tree_decode");
    exit(EXIT_FAILURE);
  }
  return_string[0] = '\0';
  int i = 0;

  while (code[i] == 'L' || code[i] == 'R') {
    huffman_tree_t *current_node = t;

    while (current_node->letter == '\0') {
      if (code[i] == 'L') {
        current_node = current_node->left;
        i++;
      } else if (code[i] == 'R') {
        current_node = current_node->right;
        i++;
      }

      if (current_node == NULL) {
        fprintf(stderr, "Found incorrect code. Exiting.\n");
        exit(EXIT_FAILURE);
      }
    }

    char current_letter[2];
    current_letter[0] = current_node->letter;
    current_letter[1] = '\0';
    strcat(return_string, current_letter);
  }

  return return_string;
}
