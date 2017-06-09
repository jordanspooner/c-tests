#include "string_utils.h"
#include "map.h"
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* Returns a heap allocated, null-terminated, zero-length string. */

char *empty_string(void)
{
  char *string = malloc(sizeof(char));
  if (string == NULL) {
    perror("empty_string");
    exit(EXIT_FAILURE);
  }
  *string = '\0';

  return string;
}


/* Given a string, return a heap-allocated copy */

char *clone(const char *str)
{
  assert(str != NULL);

  size_t string_length = (strlen(str) + 1) * sizeof(char);
  char *string_copy = malloc(string_length);
  if (string_copy == NULL) {
    perror("clone");
    exit(EXIT_FAILURE);
  }
  memcpy(string_copy, str, string_length);

  return string_copy;
}


/* Returns a heap allocated string that consists of "current" appended to
 * "append". This process must either free current or prevent it from
 * being leaked via some other mechanism.
 */

char *push_string(char *current, const char *append)
{
  assert(current != NULL);
  assert(append != NULL);

  size_t current_length = strlen(current) * sizeof(char);
  size_t append_length = (strlen(append) + 1) * sizeof(char);
  size_t return_length = current_length + append_length;

  current = realloc(current, return_length);
  if (current == NULL) {
    perror("push_string");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < append_length; i += sizeof(char)) {
    current[current_length + i] = append[i];
  }

  return current;
}


/* Removes trailing /n (if present) from str */

void trim_newline(char *str)
{
  const size_t length = strlen(str);
  if (length > 0 && str[length - 1] == '\n')
    str[length - 1] = '\0';
}


/* Transforms a string to lowercase */

void make_lowercase(char *str)
{
  assert(str != NULL);

  for(; *str != '\0'; ++str)
    *str = tolower(*str);
}


/* Given an input string, return the number of tokens, and a table of
 * tokens in *tokens. The input string is damaged by this process. The
 * returned table should be freed after use.
 */

int tokenize(char ***tokens, char* input)
{
  assert(input != NULL);

  int token_count = 0;
  int middle_of_word = 0;
  char **output = malloc(0);
  assert(output != NULL);

  while(*input != '\0')
  {
    if (*input == ' ' || *input == '.' || *input == '?' || *input == '\n')
    {
      *input = '\0';
      middle_of_word = 0;
    }
    else if (!middle_of_word)
    {
      ++token_count;
      output = realloc(output, token_count * sizeof(char*));
      assert(output != NULL);
      output[token_count - 1] = input;
      middle_of_word = 1;
    }

    ++input;
  }
  *tokens = output;
  return token_count;
}


/* Rewrites the supplied string, using the mapping from strings to
 * strings in substitutions. The returned string should be freed after
 * use.
 */

char *rewrite_string(struct map *substitutions, const char* const_input)
{
  char *const input = clone(const_input);
  char **tokens;
  char *result = empty_string();

  const int token_count = tokenize(&tokens, input);
  for(int index = 0; index < token_count; ++index)
  {
    make_lowercase(tokens[index]);
    char *replacement = (char *) map_lookup(substitutions, tokens[index]);

    if (replacement == NULL)
      result = push_string(result, tokens[index]);
    else
      result = push_string(result, replacement);

    if (index + 1 < token_count)
      result = push_string(result, " ");
  }

  free(input);
  free(tokens);
  return result;
}
